cmake_minimum_required(VERSION 2.8.3)
project(team103
)

## Add support for C++11, supported in ROS Kinetic and newer
# add_definitions(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp rospy std_msgs 
  pcl_conversions pcl_ros 
  cv_bridge image_transport tf 
  sensor_msgs geometry_msgs std_msgs visualization_msgs 
  image_geometry message_generation
  move_base_msgs actionlib
  wpb_home_bringup wpb_home_tutorials wpb_home_behaviors
  xfyun_waterplus waterplus_map_tools
)

find_package(PCL REQUIRED)
find_package(OpenCV REQUIRED)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
include_directories(
  include ${catkin_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  include
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES wpb_home_apps
  CATKIN_DEPENDS message_runtime
#  DEPENDS system_lib
)

###########
## Build ##
###########

include_directories(include ${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)


add_executable(team103
  src/team103_main.cpp
  src/gmain.cpp
  src/init.cpp
  src/function.cpp
  src/voice.cpp
  src/grab.cpp
  src/navigation.cpp
)
add_dependencies(team103 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(team103
  ${catkin_LIBRARIES}
)

# add_executable(gmain
#   src/gmain.cpp
# )
# add_dependencies(gmain ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(gmain
#   ${catkin_LIBRARIES}
# )

# add_executable(init
#   src/init.cpp
# )
# add_dependencies(init ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(init
#   ${catkin_LIBRARIES}
# )

# add_executable(function
#   src/function.cpp
# )
# add_dependencies(function ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(function
#   ${catkin_LIBRARIES}
# )