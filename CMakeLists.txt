cmake_minimum_required(VERSION 3.20)
project(holgen)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)
# TODO: probably not xplatform
add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wduplicated-branches -Wduplicated-cond)
add_subdirectory(libs)
include_directories(libs)
add_subdirectory(apps)
add_subdirectory(tests)
