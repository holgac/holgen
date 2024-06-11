set(sources
    main.cpp
    LuaContext.cpp
    example1.cpp
    example2.cpp
    example3.cpp
    example4.cpp
)

add_executable(holgen_example_usages ${sources})

add_dependencies(holgen_example_usages
    ex1_lib
    ex2_lib
    ex3_lib
    ex4_lib
    lua_lib
)
target_link_libraries(holgen_example_usages
    ex1_lib
    ex2_lib
    ex3_lib
    ex4_lib
    lua_lib
)
