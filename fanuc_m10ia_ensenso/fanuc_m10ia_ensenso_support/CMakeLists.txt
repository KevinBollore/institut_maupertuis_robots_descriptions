cmake_minimum_required(VERSION 2.8.3)

project(fanuc_m10ia_ensenso_support)

find_package(catkin REQUIRED)

catkin_package()

if (CATKIN_ENABLE_TESTING)
  find_package(roslaunch REQUIRED)
  roslaunch_add_file_check(tests/roslaunch_test.xml
    DEPENDENCIES
      # not nice, but industrial_robot_client doesn't prefix its targets
      joint_trajectory_action
      fanuc_driver_robot_state
      fanuc_driver_robot_state_bswap
      fanuc_driver_motion_streaming_interface
      fanuc_driver_motion_streaming_interface_bswap)
endif()

foreach(dir config launch meshes urdf)
  install(
    DIRECTORY ${dir}/
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach()
