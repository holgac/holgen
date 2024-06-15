set(gen_base
    CodeGenerator.cpp
    Translator.cpp
    HeaderContainer.cpp
    CodeBlock.cpp
    TypeInfo.cpp
    TranslatedProject.cpp
    NamingConvention.cpp
    Validator.cpp
    StringSwitcher.cpp
    UserDefinedSectionExtractor.cpp
)

set(gen_enum_plugins
    plugins/enum/EnumPluginBase.cpp
    plugins/enum/EnumPlugin.cpp
    plugins/enum/BitmapPlugin.cpp
)

set(gen_json_plugins
    plugins/json/JsonPlugin.cpp
    plugins/json/JsonParseFilesPlugin.cpp
    plugins/json/JsonHelperPlugin.cpp
    plugins/json/JsonConverterPlugin.cpp
)

set(gen_lua_plugins
    plugins/lua/LuaPlugin.cpp
    plugins/lua/LuaHelperPlugin.cpp
    plugins/lua/LuaFunctionPlugin.cpp
)

set(gen_plugins
    plugins/TranslatorPlugin.cpp
    plugins/helpers/GlobalPointerPlugin.cpp
    plugins/helpers/FilesystemHelperPlugin.cpp
    plugins/ClassIdFieldPlugin.cpp
    plugins/ClassFieldPlugin.cpp
    plugins/ClassFieldGetterPlugin.cpp
    plugins/ClassFieldSetterPlugin.cpp
    plugins/ContainerFieldPlugin.cpp
    plugins/ClassPlugin.cpp
    plugins/ManagedClassPlugin.cpp
    plugins/CppFunctionPlugin.cpp
    plugins/CppDestructorPlugin.cpp
    plugins/ClassEqualsOperatorPlugin.cpp
    plugins/ClassFieldVariantPlugin.cpp
    plugins/ClassNonCopyablePlugin.cpp
    plugins/ClassRuleOfFivePlugin.cpp
)

add_library(generator
    ${gen_base}
    ${gen_plugins}
    ${gen_enum_plugins}
    ${gen_json_plugins}
    ${gen_lua_plugins}
)
