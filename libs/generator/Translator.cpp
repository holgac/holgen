#include "Translator.h"
#include "parser/MonolithValidator.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldGetterPlugin.h"
#include "generator/plugins/ClassFieldSetterPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"
#include "generator/plugins/CppFunctionPlugin.h"
#include "generator/plugins/ContainerFieldPlugin.h"
#include "generator/plugins/ManagedClassPlugin.h"
#include "generator/plugins/EnumPlugin.h"
#include "generator/plugins/json/JsonPlugin.h"
#include "generator/plugins/json/JsonParseFilesPlugin.h"
#include "generator/plugins/json/JsonHelperPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "generator/plugins/lua/LuaHelperPlugin.h"
#include "generator/plugins/helpers/GlobalPointerPlugin.h"
#include "generator/plugins/helpers/FilesystemHelperPlugin.h"

namespace holgen {

  Translator::Translator(const ProjectDefinition &project) : mProject(project) {
    MonolithValidator(project).Validate();
    // TODO: currently plugins iterate over struct fields, but we need to iterate over class fields
    // and filter by mField to avoid processing custom fields
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
