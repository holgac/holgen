set(gen_base
    types/CppTypes.cpp
    types/CSharpTypes.cpp
    CodeGenerator.cpp
    Translator.cpp
    CodeBlock.cpp
    TypeInfo.cpp
    TranslatedProject.cpp
    Validator.cpp
    UserDefinedSectionExtractor.cpp
)

set(gen_enum_plugins
    plugins/enum/EnumPluginBase.cpp
    plugins/enum/EnumPlugin.cpp
    plugins/enum/BitmapPlugin.cpp
    plugins/enum/BitmapFieldPlugin.cpp
)

set(gen_json_plugins
    plugins/json/JsonPluginBase.cpp
    plugins/json/JsonParsePlugin.cpp
    plugins/json/JsonDumpPlugin.cpp
    plugins/json/JsonParseFilesPlugin.cpp
    plugins/json/JsonDumpFilesPlugin.cpp
    plugins/json/JsonHelperPlugin.cpp
    plugins/json/JsonConverterPlugin.cpp
)

set(gen_lua_plugins
    plugins/lua/LuaPluginBase.cpp
    plugins/lua/LuaIndexMetaMethodPlugin.cpp
    plugins/lua/LuaPlugin.cpp
    plugins/lua/LuaFieldsPlugin.cpp
    plugins/lua/LuaHelperPlugin.cpp
    plugins/lua/LuaFunctionPlugin.cpp
)

set(gen_csharp_plugins
    plugins/csharp/DotNetClassPlugin.cpp
    plugins/csharp/DotNetModulePlugin.cpp
    plugins/csharp/DotNetModuleFunctionsPlugin.cpp
    plugins/csharp/DotNetHostPlugin.cpp
    plugins/csharp/DotNetModuleInterfacePlugin.cpp
    plugins/csharp/DotNetWrapperPlugin.cpp
    plugins/csharp/DotNetInterfacePlugin.cpp
    plugins/csharp/DotNetInterfaceClassPlugin.cpp
    plugins/csharp/DotNetDeferredDeleterPlugin.cpp
)

set(gen_plugins
    plugins/TranslatorPlugin.cpp
    plugins/FunctionPluginBase.cpp
    plugins/helpers/GlobalPointerPlugin.cpp
    plugins/helpers/FilesystemHelperPlugin.cpp
    plugins/helpers/SingletonPlugin.cpp
    plugins/helpers/DeferredDeleterPlugin.cpp
    plugins/cpp/ClassIdFieldPlugin.cpp
    plugins/cpp/ClassFieldPlugin.cpp
    plugins/cpp/ClassFieldGetterPlugin.cpp
    plugins/cpp/ClassFieldSetterPlugin.cpp
    plugins/cpp/ClassPlugin.cpp
    plugins/cpp/ManagedClassPlugin.cpp
    plugins/cpp/CppFunctionPlugin.cpp
    plugins/cpp/CppDestructorPlugin.cpp
    plugins/cpp/ClassEqualsOperatorPlugin.cpp
    plugins/cpp/ClassFieldVariantPlugin.cpp
    plugins/cpp/ClassCopyMoveDestroyPlugin.cpp
    plugins/cpp/ClassMetadataPlugin.cpp
    plugins/cpp/CWrappersPlugin.cpp
    plugins/cpp/CompositeIdTypePlugin.cpp
    plugins/cpp/CompositeIdStructPlugin.cpp
    plugins/container/ContainerPluginBase.cpp
    plugins/container/ContainerIndexPlugin.cpp
    plugins/container/ContainerAddElemPlugin.cpp
    plugins/container/ContainerGetElemPlugin.cpp
    plugins/container/ContainerDeleteElemPlugin.cpp
    plugins/container/ContainerFieldPlugin.cpp
)

set(gen_generators
    generators/Generator.cpp
    generators/CMakeGenerator.cpp
    generators/HolgenHeaderGenerator.cpp
    generators/CppModifiableSourceGenerator.cpp
    generators/CppHeaderGenerator.cpp
    generators/CppSourceGenerator.cpp
    generators/CSharpProjectGenerator.cpp
    generators/CSharpHolgenMainGenerator.cpp
    generators/CSharpSourceGenerator.cpp
    generators/CSharpEnumGenerator.cpp
    generators/LuaGeneratorBase.cpp
    generators/LuaSourceGenerator.cpp
    generators/LuaPublisherGenerator.cpp
)

set(gen_utils
    utils/CSharpHelper.cpp
    utils/CSharpMethodHelper.cpp
    utils/BridgingHelper.cpp
    utils/HeaderContainer.cpp
    utils/NamingConvention.cpp
    utils/StringSwitcher.cpp
    utils/CompositeIdHelper.cpp
)

add_library(generator
    ${gen_base}
    ${gen_plugins}
    ${gen_enum_plugins}
    ${gen_json_plugins}
    ${gen_lua_plugins}
    ${gen_csharp_plugins}
    ${gen_utils}
    ${gen_generators}
)
