add_executable(tests test_tokenizer.cpp test_parser.cpp test_translator.cpp test_generator.cpp test_validator.cpp test_dependency_graph.cpp)
add_dependencies(tests gtest gtest_main generator parser tokenizer core)
target_link_libraries(tests gtest gtest_main generator parser tokenizer core)
