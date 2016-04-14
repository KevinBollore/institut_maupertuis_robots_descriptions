cmake_minimum_required(VERSION 2.8.3)
project(fanuc_r1000ia_sls_2_grinding_resources)

find_package(catkin REQUIRED COMPONENTS
             cv_bridge
             moveit_ros_planning_interface
             roscpp
             tf_conversions
             visualization_msgs)

find_package(PCL 1.8.0 REQUIRED)

catkin_package()

include_directories(${PCL_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS}) # Order matters!
add_definitions(${PCL_DEFINITIONS})

add_executable(test_calibration src/test_calibration.cpp)
set_property(TARGET test_calibration APPEND_STRING PROPERTY COMPILE_FLAGS -Wall)
target_link_libraries(test_calibration ${PCL_LIBRARIES} ${catkin_LIBRARIES} yaml-cpp)
