cmake_minimum_required(VERSION 3.20)
project(holgen_blackbox_csharp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(x_googletest)
include_directories(x_googletest/googletest/include)
include_directories(x_googletest/googlemock/include)

include_directories(tests)
add_subdirectory(output)
include_directories(output/gen)

add_subdirectory(tests)