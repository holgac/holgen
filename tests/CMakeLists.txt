set(sources
    Helpers.cpp
    test_tokenizer.cpp
    test_parser.cpp
    test_generator.cpp
    test_dependency_graph.cpp
    test_generator_global_pointer.cpp
    UserDefinedSectionExtractorTest.cpp
)
set(translator_plugin_sources
    plugins/ClassPluginTest.cpp
    plugins/ClassIdFieldPluginTest.cpp
    plugins/ClassFieldPluginTest.cpp
    plugins/ClassFieldGetterPluginTest.cpp
    plugins/ClassFieldSetterPluginTest.cpp
    plugins/LuaFunctionPluginTest.cpp
    plugins/CppFunctionPluginTest.cpp
    plugins/ContainerFieldPluginTest.cpp
    plugins/ManagedClassPluginTest.cpp
    plugins/EnumPluginTest.cpp
    plugins/JsonParsePluginTest.cpp
    plugins/JsonParseFilesPluginTest.cpp
    plugins/JsonConverterPluginTest.cpp
    plugins/LuaPluginTest.cpp
    plugins/ClassEqualsOperatorPluginTest.cpp
    plugins/ClassFieldVariantPluginTest.cpp
)
add_executable(holgen_tests ${sources} ${translator_plugin_sources})
add_dependencies(holgen_tests gtest gtest_main generator parser tokenizer core)
target_link_libraries(holgen_tests gtest gtest_main generator parser tokenizer core)
