// ROS
#include <ros/ros.h>
#include <ros/package.h>
#include <tf/transform_listener.h>
#include <tf_conversions/tf_eigen.h>
#include <eigen_conversions/eigen_msg.h>
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_interface/planning_interface.h>
#include <moveit_msgs/ExecuteKnownTrajectory.h>
#include <visualization_msgs/Marker.h>
#include <pcl_ros/point_cloud.h>
#include "yaml_utils.h"

// PCL headers
#include <pcl/point_cloud.h>
#include <pcl/common/colors.h>
#include <pcl/common/transforms.h>
#include <pcl/io/davidsdk_grabber.h>
#include <pcl/io/ply_io.h>
//#include <pcl/filters/voxel_grid.h>

// PCL davidSDK object pointer
pcl::DavidSDKGrabber::Ptr davidsdk_ptr;

typedef pcl::PointXYZ PointXYZ;
typedef pcl::PointCloud<PointXYZ> PointCloudXYZ;

/** @brief The main function
 * @param[in] argc
 * @param[in] argc
 * @return Exit status */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_calibration");
  ros::NodeHandle node;
  tf::TransformListener listener;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  move_group_interface::MoveGroup group("manipulator");
  group.setPoseReferenceFrame("/base_link");
  group.setPlannerId("RRTConnectkConfigDefault");
  group.setPlanningTime(1);

  // Declare 3 joint states
  std::vector<double> pose_1;
  std::vector<double> pose_2;
  std::vector<double> pose_3;

  // TODO: Use a yaml parser in order to fill joint values
  pose_1.push_back(-0.4125434753);
  pose_1.push_back(0.0522551578);
  pose_1.push_back(-0.596710618);
  pose_1.push_back(3.0800174376);
  pose_1.push_back(1.2996419742);
  pose_1.push_back(1.8479197054);

  pose_2.push_back(-0.3132342409);
  pose_2.push_back(0.1120152314);
  pose_2.push_back(-0.3339687524);
  pose_2.push_back(3.0820420195);
  pose_2.push_back(1.616750846);
  pose_2.push_back(1.6149357036);

  pose_3.push_back(-0.1891587843);
  pose_3.push_back(0.2655343924);
  pose_3.push_back(-0.3844436743);
  pose_3.push_back(2.8598965123);
  pose_3.push_back(1.270931308);
  pose_3.push_back(1.3584944766);

  // Get davidSDK ip and server name
  std::string davidSDK_ip;
  std::string davidSDK_server_name;
  node.getParam("/davidSDK_ip_param", davidSDK_ip);
  node.getParam("/davidSDK_server_name_param", davidSDK_server_name);

  // Connect to the SLS_2
  davidsdk_ptr.reset(new pcl::DavidSDKGrabber);
  ROS_INFO_STREAM("Trying to connect to DavidSDK IP \"" << davidSDK_ip << "\" and server name \"" << davidSDK_server_name << "\"");
  davidsdk_ptr->connect(davidSDK_ip);
  if (!davidsdk_ptr->isConnected())
  {
    ROS_ERROR_STREAM("Cannot connect to the David SLS-2 scanner (IP: " << davidSDK_ip << ")");
    ROS_ERROR_STREAM("Server name: " << davidSDK_server_name);
    return -1;
  }

  davidsdk_ptr->setLocalAndRemotePaths ("/var/tmp/davidsdk/", "\\\\"+davidSDK_server_name+"\\davidsdk\\");
  davidsdk_ptr->setFileFormatToSTL(); // Best performance

  // Parse YAML file in order to obtain sls_2_calibration matrix
  // Get package path
  std::string package_path = ros::package::getPath("fanuc_r1000ia_sls_2_grinding_resources");
  std::string sls_2_calib_file = package_path + "/yaml/sls_2_calibration.yaml";

  YAML::Node yamlFileCalibration;
  if(!yaml_parser::yamlNodeFromFileName(sls_2_calib_file, yamlFileCalibration))
  {
    ROS_ERROR_STREAM("Cannot find yaml file: " << sls_2_calib_file);
    return -1;
  }

  // Parse node in YAML file
  const YAML::Node& sls_2_node = yaml_parser::parseNode(yamlFileCalibration, "sls_2_calibration_values");
  if(!yamlFileCalibration["sls_2_calibration_values"])
  {
    ROS_ERROR_STREAM("Cannot parse node 'sls_2_calibration_values'");
    return -1;
  }

  // Parse all tags in "sls_2_calibration_values" node in YAML file
  std::vector<double> n_vector;
  std::vector<double> o_vector;
  std::vector<double> a_vector;
  std::vector<double> p_vector;
  yaml_parser::parseVectorD(sls_2_node[0], "n", n_vector);
  yaml_parser::parseVectorD(sls_2_node[1], "o", o_vector);
  yaml_parser::parseVectorD(sls_2_node[2], "a", a_vector);
  yaml_parser::parseVectorD(sls_2_node[3], "p", p_vector);

  Eigen::Affine3d calib_sls2;

  // We fill the calibration_sls2 tranformation matrix with values contained in yaml file
  calib_sls2.matrix() <<
      n_vector[0], o_vector[0], a_vector[0], p_vector[0],
      n_vector[1], o_vector[1], a_vector[1], p_vector[1],
      n_vector[2], o_vector[2], a_vector[2], p_vector[2],
      0          , 0          , 0          , 1;
  // We have to invert this matrix in order to give point cloud in a correct frame
  calib_sls2 = calib_sls2.inverse();

  tf::StampedTransform transform;
  // Go to pose 1
  group.setJointValueTarget(pose_1);
  group.move();
  sleep(1);
  listener.waitForTransform("/base", "/sls_2_frame", ros::Time::now(), ros::Duration(1.5)); // Wait for the current SLS-2 pose to come
  listener.lookupTransform("/base_link", "/sls_2_frame", ros::Time(0), transform);
  Eigen::Affine3d matrix_transform_1;
  tf::transformTFToEigen(transform, matrix_transform_1);
  // Grab first single cloud
  PointCloudXYZ::Ptr pc1 (new PointCloudXYZ());
  davidsdk_ptr->grabSingleCloud(*pc1);
  ROS_INFO_STREAM("Point cloud 1 captured, SLS-2 frame pose is:\n" << matrix_transform_1.matrix());

  // Go to pose 2
  group.setJointValueTarget(pose_2);
  group.move();
  sleep(1);
  listener.waitForTransform("/base", "/sls_2_frame", ros::Time::now(), ros::Duration(1.5));
  listener.lookupTransform("/base_link", "/sls_2_frame", ros::Time(0), transform);
  Eigen::Affine3d matrix_transform_2;
  tf::transformTFToEigen(transform, matrix_transform_2);
  // Grab second single cloud
  PointCloudXYZ::Ptr pc2 (new PointCloudXYZ());
  davidsdk_ptr->grabSingleCloud(*pc2);
  ROS_INFO_STREAM("Point cloud 2 captured, SLS-2 frame pose is:\n" << matrix_transform_2.matrix());

  // Go to pose 3
  group.setJointValueTarget(pose_3);
  group.move();
  sleep(1);
  listener.waitForTransform("/base", "/sls_2_frame", ros::Time::now(), ros::Duration(1.5));
  listener.lookupTransform("/base_link", "/sls_2_frame", ros::Time(0), transform);
  Eigen::Affine3d matrix_transform_3;
  tf::transformTFToEigen(transform, matrix_transform_3);
  // Grab third single cloud
  PointCloudXYZ::Ptr pc3 (new PointCloudXYZ());
  davidsdk_ptr->grabSingleCloud(*pc3);
  ROS_INFO_STREAM("Point cloud 3 captured, SLS-2 frame pose is:\n" << matrix_transform_3.matrix());

  // Create the filtering object
  /*ROS_INFO_STREAM("Filtering point clouds (voxel grid)");
  pcl::VoxelGrid<PointXYZ> sor;
  double leaf_size (5); // Point cloud is not yet resized in meters
  sor.setLeafSize (leaf_size, leaf_size, leaf_size);
  sor.setInputCloud (pc1);
  sor.filter (*pc1);
  sor.setInputCloud (pc2);
  sor.filter (*pc2);
  sor.setInputCloud (pc3);
  sor.filter (*pc3);*/

  // Compute transformation matrix for each point cloud
  Eigen::Affine3d transformation_pc1(Eigen::Affine3d::Identity());
  transformation_pc1 = matrix_transform_1 * calib_sls2;
  transformation_pc1.matrix() *= 0.001; // SLS-2 meshes are stored in milimeters

  Eigen::Affine3d transformation_pc2(Eigen::Affine3d::Identity());
  transformation_pc2 = matrix_transform_2 * calib_sls2;
  transformation_pc1.matrix() *= 0.001;

  Eigen::Affine3d transformation_pc3(Eigen::Affine3d::Identity());
  transformation_pc3 = matrix_transform_3 * calib_sls2;
  transformation_pc1.matrix() *= 0.001;

  ROS_INFO_STREAM("Transforming point clouds");
  pcl::transformPointCloud(*pc1, *pc1, transformation_pc1);
  pcl::transformPointCloud(*pc2, *pc2, transformation_pc2);
  pcl::transformPointCloud(*pc3, *pc3, transformation_pc3);

  ROS_INFO_STREAM("Saving PLY files");
  pcl::io::savePLYFile("SavePC1.ply", *pc1);
  pcl::io::savePLYFile("SavePC2.ply", *pc2);
  pcl::io::savePLYFile("SavePC3.ply", *pc3);

  // Publish all the point clouds
  ROS_INFO_STREAM("Publish point clouds");
  ros::Publisher pointcloud_pub_1;
  pointcloud_pub_1 = node.advertise<sensor_msgs::PointCloud2>("pc1", 1);
  ros::Publisher pointcloud_pub_2;
  pointcloud_pub_2 = node.advertise<sensor_msgs::PointCloud2>("pc2", 1);
  ros::Publisher pointcloud_pub_3;
  pointcloud_pub_3 = node.advertise<sensor_msgs::PointCloud2>("pc3", 1);

  // Display point clouds
  while (pointcloud_pub_1.getNumSubscribers() < 1 ||
         pointcloud_pub_2.getNumSubscribers() < 1 ||
         pointcloud_pub_3.getNumSubscribers() < 1)
  {
    ROS_WARN_STREAM("Please create 3 subscribers to the markers");
    sleep(1);
    if (!ros::ok())
      break;
  }

  pc1->header.frame_id =
  pc2->header.frame_id =
  pc3->header.frame_id = "base_link";

  pointcloud_pub_1.publish(pc1);
  pointcloud_pub_2.publish(pc2);
  pointcloud_pub_3.publish(pc3);

  while(node.ok())
  {}

  return 0;
}
