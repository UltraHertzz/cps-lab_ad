cmake_minimum_required(VERSION 2.8.3)
project(proj_package)

find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

generate_messages(DEPENDENCIES std_msgs)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(key_board_teleop src/key_board_teleop.cpp)
add_executable(pid src/pid.cpp)
add_executable(mux src/mux.cpp)
target_link_libraries(key_board_teleop ${catkin_LIBRARIES})
target_link_libraries(pid ${catkin_LIBRARIES})
target_link_libraries(mux ${catkin_LIBRARIES})
add_dependencies(key_board_teleop proj_package_generate_messages_cpp)
add_dependencies(pid proj_package_generate_messages_cpp)
add_dependencies(mux proj_package_generate_messages_cpp)