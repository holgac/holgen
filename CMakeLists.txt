cmake_minimum_required(VERSION 3.20)
project(holgen)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)
add_subdirectory(libs)
include_directories(libs)
add_subdirectory(apps)
add_subdirectory(tests)
