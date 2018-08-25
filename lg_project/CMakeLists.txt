cmake_minimum_required(VERSION 2.8.3)
project(lg_project)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

 add_message_files(
   FILES
   Key.msg
#   Message2.msg
 )

generate_messages(
  DEPENDENCIES
  std_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES publish_subscribe_demo
  CATKIN_DEPENDS  roscpp rospy std_msgs message_runtime
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)



add_executable(timers src/timers.cpp)
target_link_libraries(timers ${catkin_LIBRARIES})

add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})

add_executable(keyloop src/keyloop.cpp)
add_dependencies(keyloop lg_project_generate_messages_cpp)
target_link_libraries(keyloop ${catkin_LIBRARIES})
