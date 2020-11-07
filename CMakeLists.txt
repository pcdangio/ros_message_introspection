cmake_minimum_required(VERSION 3.0.2)
project(message_introspection)

# Find catkin and package dependencies.
find_package(catkin REQUIRED COMPONENTS
  roscpp
  topic_tools
  rosbag
)

# Generate catkin package.
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES message_introspection
  CATKIN_DEPENDS roscpp topic_tools rosbag
)

# Specify include directories.
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

# Build library.
add_library(${PROJECT_NAME}
  src/definition.cpp
  src/definition_tree.cpp
  src/introspector.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

# Set up install target.
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)
