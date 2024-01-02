add_executable(cli main.cpp)
add_dependencies(cli tokenizer parser generator)
target_link_libraries(cli tokenizer parser generator)
