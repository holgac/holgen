cmake_minimum_required(VERSION 3.20)
project(holgen_test)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(x_googletest)
include_directories(x_googletest/googletest/include)
include_directories(x_googletest/googlemock/include)
add_subdirectory(x_rapidjson)
include_directories(x_rapidjson/include)
add_subdirectory(x_lua)
include_directories(x_lua/src)
include_directories(x_lua/include)

add_subdirectory(output)
include_directories(output/gen)
add_subdirectory(tests)
