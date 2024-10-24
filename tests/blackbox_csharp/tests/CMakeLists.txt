add_executable( tests
    test_vector.cpp
)
add_dependencies(tests gtest gtest_main gmock holgen_blackbox_swig_lib)
target_link_libraries(tests gtest gtest_main gmock holgen_blackbox_swig_lib)
