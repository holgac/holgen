add_executable(tests test_tokenizer.cpp test_parser.cpp test_translator.cpp)
add_dependencies(tests gtest gtest_main tokenizer parser generator)
target_link_libraries(tests gtest gtest_main tokenizer parser generator)