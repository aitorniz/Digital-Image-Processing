cmake_minimum_required(VERSION 3.22.1)
project(learncpp)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(learncpp main.cpp)

target_link_libraries( learncpp ${OpenCV_LIBS})



