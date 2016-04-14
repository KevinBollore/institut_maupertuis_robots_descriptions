cmake_minimum_required(VERSION 2.8.3)
project(sls_2_publishers)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp
                                        cv_bridge)
find_package(PCL 1.8.0 REQUIRED)

## Declare a catkin package
catkin_package()

include_directories(include
                    ${PCL_INCLUDE_DIRS}  # Order matters!
                    ${catkin_INCLUDE_DIRS})

add_executable(sls_2_images src/sls_2_images.cpp)
target_link_libraries(sls_2_images ${catkin_LIBRARIES} ${PCL_LIBRARIES})

