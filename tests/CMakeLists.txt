add_executable(tests test_tokenizer.cpp test_parser.cpp)
add_dependencies(tests gtest_main tokenizer parser)
target_link_libraries(tests gtest gtest_main tokenizer parser)