#include "Translator.h"
#include "parser/Validator.h"
#include "generator/translator_plugins/ClassPlugin.h"
#include "generator/translator_plugins/ClassFieldPlugin.h"
#include "generator/translator_plugins/ClassFieldGetterPlugin.h"
#include "generator/translator_plugins/ClassFieldSetterPlugin.h"
#include "generator/translator_plugins/ContainerFieldPlugin.h"
#include "generator/translator_plugins/ManagedClassPlugin.h"
#include "generator/translator_plugins/EnumPlugin.h"
#include "generator/translator_plugins/JsonPlugin.h"
#include "generator/translator_plugins/JsonParseFilesPlugin.h"
#include "generator/translator_plugins/LuaPlugin.h"
#include "generator/translator_plugins/GlobalPointerPlugin.h"
#include "generator/translator_plugins/FilesystemHelperPlugin.h"

namespace holgen {

  Translator::Translator(const ProjectDefinition &project) : mProject(project) {
    Validator(project).Validate();
    AddPlugin<ClassPlugin>();
    AddPlugin<ClassFieldPlugin>();
    AddPlugin<ClassFieldGetterPlugin>();
    AddPlugin<ClassFieldSetterPlugin>();
    AddPlugin<ContainerFieldPlugin>();
    AddPlugin<ManagedClassPlugin>();
    AddPlugin<EnumPlugin>();
    AddPlugin<JsonPlugin>();
    AddPlugin<JsonParseFilesPlugin>();
    AddPlugin<LuaPlugin>();
    AddPlugin<GlobalPointerPlugin>();
    AddPlugin<FilesystemHelperPlugin>();
  }

  TranslatedProject Translator::Translate() {
    for (const auto &plugin: mPlugins) {
      plugin->EnrichClasses();
    }

    for (const auto &plugin: mPlugins) {
      plugin->GenerateHelpers();
    }

    return mProject;
  }
}
