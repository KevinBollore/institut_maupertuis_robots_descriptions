// ROS headers
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>

// PCL headers
#include <pcl/io/davidsdk_grabber.h>

/** @brief PCL davidSDK object pointer */
pcl::DavidSDKGrabber::Ptr sls2_ptr;

/** Image publisher */
boost::shared_ptr<ros::Publisher> image_pub;

/** @brief Process and/or display SLS-2 grabber image
 * @param[in] image SLS-2 image */
void
grabberCallback (const boost::shared_ptr<pcl::PCLImage>& image)
{
  unsigned char *image_array = reinterpret_cast<unsigned char *> (&image->data[0]);
  cv::Mat cv_image (image->height, image->width, CV_8UC1, image_array);
  // Publish image on the topic
  image_pub->publish(cv_bridge::CvImage(std_msgs::Header(), "mono8", cv_image).toImageMsg());
}

/** @brief The main function */
int main(int argc, char **argv)
{
  ros::init(argc, argv, "sls_2_images");
  ros::NodeHandle node;

  image_pub.reset(new ros::Publisher);

  *image_pub = node.advertise<sensor_msgs::Image>("sls_2_image", 2);

  std::string davidSDK_ip;
  node.getParam("davidSDK_ip_param", davidSDK_ip);
  ROS_INFO_STREAM("davidSDK server IP: " << davidSDK_ip);

  sls2_ptr.reset(new pcl::DavidSDKGrabber);
  sls2_ptr->connect(davidSDK_ip);
  if (!sls2_ptr->isConnected())
  {
    ROS_ERROR_STREAM("Cannot connect to the David SLS-2 scanner (server IP: " << davidSDK_ip << ")");
    return -1;
  }

  boost::function<void (const boost::shared_ptr<pcl::PCLImage> &)> f =
      boost::bind (&grabberCallback, _1);
  sls2_ptr->registerCallback (f);
  ROS_INFO_STREAM("davidSDK publishing images on topic \'sls_2_image\'");
  sls2_ptr->start ();  // Image mode! No cloud information

  while (node.ok())
  {}

  sls2_ptr->stop ();
  return 0;
}
