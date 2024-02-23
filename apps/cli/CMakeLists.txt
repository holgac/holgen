add_executable(cli main.cpp)
add_dependencies(cli tokenizer parser generator core)
target_link_libraries(cli tokenizer parser generator core)
