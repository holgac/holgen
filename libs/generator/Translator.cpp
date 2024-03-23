#include "Translator.h"
#include "parser/Validator.h"
#include "generator/translator_plugins/ClassPlugin.h"
#include "generator/translator_plugins/ClassFieldPlugin.h"
#include "generator/translator_plugins/ClassFieldGetterPlugin.h"
#include "generator/translator_plugins/ClassFieldSetterPlugin.h"
#include "generator/translator_plugins/lua/LuaFunctionPlugin.h"
#include "generator/translator_plugins/CppFunctionPlugin.h"
#include "generator/translator_plugins/ContainerFieldPlugin.h"
#include "generator/translator_plugins/ManagedClassPlugin.h"
#include "generator/translator_plugins/EnumPlugin.h"
#include "generator/translator_plugins/json/JsonPlugin.h"
#include "generator/translator_plugins/json/JsonParseFilesPlugin.h"
#include "generator/translator_plugins/json/JsonHelperPlugin.h"
#include "generator/translator_plugins/json/JsonConverterPlugin.h"
#include "generator/translator_plugins/lua/LuaPlugin.h"
#include "generator/translator_plugins/lua/LuaHelperPlugin.h"
#include "generator/translator_plugins/GlobalPointerPlugin.h"
#include "generator/translator_plugins/FilesystemHelperPlugin.h"

namespace holgen {

  Translator::Translator(const ProjectDefinition &project) : mProject(project) {
    Validator(project).Validate();
    AddPlugin<ClassPlugin>();
    AddPlugin<ClassFieldPlugin>();
    AddPlugin<ClassFieldGetterPlugin>();
    AddPlugin<ClassFieldSetterPlugin>();
    AddPlugin<LuaFunctionPlugin>();
    AddPlugin<CppFunctionPlugin>();
    AddPlugin<ContainerFieldPlugin>();
    AddPlugin<ManagedClassPlugin>();
    AddPlugin<EnumPlugin>();
    AddPlugin<JsonPlugin>();
    AddPlugin<JsonParseFilesPlugin>();
    AddPlugin<JsonHelperPlugin>();
    AddPlugin<JsonConverterPlugin>();
    AddPlugin<LuaPlugin>();
    AddPlugin<LuaHelperPlugin>();
    AddPlugin<GlobalPointerPlugin>();
    AddPlugin<FilesystemHelperPlugin>();
  }

  TranslatedProject Translator::Translate() {
    for (const auto &plugin: mPlugins) {
      plugin->Run();
    }
    return mProject;
  }
}
