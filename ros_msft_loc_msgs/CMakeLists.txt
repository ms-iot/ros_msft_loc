cmake_minimum_required(VERSION 2.8.3)
project(ros_msft_loc_msgs)

find_package(catkin REQUIRED COMPONENTS message_generation std_msgs sensor_msgs)


## Generate messages in the 'msg' folder
add_message_files(
   FILES
   NavSatFixPrecise.msg
)
## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   std_msgs
   sensor_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
   CATKIN_DEPENDS message_runtime std_msgs sensor_msgs
)
