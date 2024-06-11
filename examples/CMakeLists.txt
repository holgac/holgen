cmake_minimum_required(VERSION 3.20)
project(holgen_examples)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)
# Normally a subdirectory would add its children, but the external subdirectory is a symlink to the main external subdirectory of the repo where we don't want to add lua or rapidjson dependencies.
add_subdirectory(external/x_lua)
include_directories(external/x_lua/include)
include_directories(external/x_lua/src)
include_directories(external/x_rapidjson/include)
if (UNIX)
  add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wduplicated-branches -Wduplicated-cond)
endif ()

# Include the generated projects
add_subdirectory(1/output)
add_subdirectory(2/output)
add_subdirectory(3/output)
add_subdirectory(4/output)
include_directories(.)


add_subdirectory(usage)
