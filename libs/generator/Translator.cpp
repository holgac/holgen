#include "Translator.h"
#include "generator/plugins/ClassEqualsOperatorPlugin.h"
#include "generator/plugins/ClassFieldGetterPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldSetterPlugin.h"
#include "generator/plugins/ClassFieldVariantPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"
#include "generator/plugins/ClassNonCopyablePlugin.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ContainerFieldPlugin.h"
#include "generator/plugins/CppDestructorPlugin.h"
#include "generator/plugins/CppFunctionPlugin.h"
#include "generator/plugins/enum/EnumPlugin.h"
#include "generator/plugins/enum/BitmapPlugin.h"
#include "generator/plugins/ManagedClassPlugin.h"
#include "generator/plugins/helpers/FilesystemHelperPlugin.h"
#include "generator/plugins/helpers/GlobalPointerPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/json/JsonHelperPlugin.h"
#include "generator/plugins/json/JsonParseFilesPlugin.h"
#include "generator/plugins/json/JsonPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"
#include "generator/plugins/lua/LuaHelperPlugin.h"
#include "generator/plugins/lua/LuaFieldsPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "generator/plugins/ClassRuleOfFivePlugin.h"

namespace holgen {

Translator::Translator(const TranslatorSettings &translatorSettings) :
    mTranslatorSettings(translatorSettings) {
  // TODO: Users should pick which plugins to run
  AddPlugin<ClassPlugin>();
  AddPlugin<ClassNonCopyablePlugin>();
  AddPlugin<ClassIdFieldPlugin>();
  AddPlugin<ClassFieldPlugin>();
  AddPlugin<ClassEqualsOperatorPlugin>();
  AddPlugin<ClassFieldGetterPlugin>();
  AddPlugin<ClassFieldSetterPlugin>();
  AddPlugin<ClassFieldVariantPlugin>();
  AddPlugin<LuaFunctionPlugin>();
  AddPlugin<CppFunctionPlugin>();
  AddPlugin<CppDestructorPlugin>();
  AddPlugin<ContainerFieldPlugin>();
  AddPlugin<ManagedClassPlugin>();
  AddPlugin<EnumPlugin>();
  AddPlugin<BitmapPlugin>();
  AddPlugin<JsonConverterPlugin>();
  AddPlugin<JsonPlugin>();
  AddPlugin<JsonParseFilesPlugin>();
  AddPlugin<JsonHelperPlugin>();
  // TODO: lua plugins should run before json so that LuaRegistryData can be initialized via json
  AddPlugin<LuaFieldsPlugin>();
  AddPlugin<LuaPlugin>();
  AddPlugin<LuaHelperPlugin>();
  AddPlugin<GlobalPointerPlugin>();
  AddPlugin<FilesystemHelperPlugin>();
  AddPlugin<ClassRuleOfFivePlugin>();
}

TranslatedProject Translator::Translate(const ProjectDefinition &project) const {
  TranslatedProject translatedProject(project);
  for (const auto &plugin: mPlugins) {
    plugin(translatedProject);
  }
  return translatedProject;
}
} // namespace holgen
