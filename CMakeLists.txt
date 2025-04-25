cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

project(BagFromImages)

rosbuild_init()

# Find required packages
find_package(OpenCV REQUIRED)
find_package(console_bridge REQUIRED)

# Set executable output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Add your executable
rosbuild_add_executable(${PROJECT_NAME}
  main.cc
  Thirdparty/DLib/FileFunctions.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
  ${console_bridge_LIBRARIES}
  -lconsole_bridge  # Link console_bridge explicitly
)
