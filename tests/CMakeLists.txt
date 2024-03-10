add_executable(holgen_tests test_tokenizer.cpp test_parser.cpp test_translator.cpp test_generator.cpp test_validator.cpp test_dependency_graph.cpp test_generator_global_pointer.cpp Helpers.cpp)
add_dependencies(holgen_tests gtest gtest_main generator parser tokenizer core)
target_link_libraries(holgen_tests gtest gtest_main generator parser tokenizer core)
