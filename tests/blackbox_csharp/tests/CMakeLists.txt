
# Find the hostfxr library
find_library(HOSTFXR_LIB
    NAMES hostfxr
    PATH_SUFFIXES "8.0.8"
    PATHS /usr/lib64/dotnet/host/fxr
    # PATHS ${PROJECT_SOURCE_DIR}/bin/TestModule"
    NO_DEFAULT_PATH
)

# Find the nethost library
find_library(NETHOST_LIB
    NAMES nethost
    PATHS /usr/lib64/dotnet/packs/Microsoft.NETCore.App.Host.fedora.40-x64/6.0.33/runtimes/fedora.40-x64/native/
    # PATHS ${PROJECT_SOURCE_DIR}/bin/TestModule"
    NO_DEFAULT_PATH
)

# Error if hostfxr is not found
if (NOT HOSTFXR_LIB)
  message(FATAL_ERROR "hostfxr library not found!")
else ()
  message(STATUS "Found hostfxr: ${HOSTFXR_LIB}")
endif ()

# Error if hostfxr is not found
if (NOT NETHOST_LIB)
  message(FATAL_ERROR "nethost library not found!")
else ()
  message(STATUS "Found nethost: ${NETHOST_LIB}")
endif ()


add_executable(tests
    test_vector.cpp
    test_module.cpp
)
add_dependencies(tests gtest gtest_main gmock holgen_blackbox_csharp_lib)
target_link_libraries(tests gtest gtest_main gmock holgen_blackbox_csharp_lib ${HOSTFXR_LIB} ${NETHOST_LIB})
