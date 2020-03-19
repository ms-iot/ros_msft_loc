cmake_minimum_required(VERSION 2.8.3)
project(ros_msft_loc)

## Compile as at least C++17 for cppwinrt
add_compile_options(/Od)
add_compile_options(/std:c++latest)
add_compile_options(/await)
add_definitions("/D_ENABLE_EXTENDED_ALIGNED_STORAGE")
add_definitions("/D_USE_MATH_DEFINES")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
  std_msgs
  Eigen3 
  sensor_msgs
  ros_msft_loc_msgs
)
include_directories(${catkin_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})
add_definitions(${EIGEN3_DEFINITIONS})

catkin_package(
  INCLUDE_DIRS
  LIBRARIES ros_msft_loc
  CATKIN_DEPENDS sensor_msgs roscpp roslib std_msgs sensor_msgs ros_msft_loc_msgs
  DEPENDS
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/main.cpp)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})

install(TARGETS ${PROJECT_NAME}_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch testdata
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
