cmake_minimum_required(VERSION 3.23)
project(Sort_Visualizer)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(Sort_Visualizer ${SDL2_INCLUDE_DIRS})

add_executable(Sort_Visualizer main.cpp)
target_link_libraries(Sort_Visualizer ${SDL2_LIBRARIES})
