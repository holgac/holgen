#include "Translator.h"
#include "generator/plugins/ClassEqualsOperatorPlugin.h"
#include "generator/plugins/ClassFieldGetterPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldSetterPlugin.h"
#include "generator/plugins/ClassFieldVariantPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ContainerFieldPlugin.h"
#include "generator/plugins/CppDestructorPlugin.h"
#include "generator/plugins/CppFunctionPlugin.h"
#include "generator/plugins/enum/EnumPlugin.h"
#include "generator/plugins/enum/BitmapPlugin.h"
#include "generator/plugins/enum/BitmapFieldPlugin.h"
#include "generator/plugins/ManagedClassPlugin.h"
#include "generator/plugins/helpers/FilesystemHelperPlugin.h"
#include "generator/plugins/helpers/GlobalPointerPlugin.h"
#include "generator/plugins/helpers/SingletonPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/json/JsonHelperPlugin.h"
#include "generator/plugins/json/JsonParseFilesPlugin.h"
#include "generator/plugins/json/JsonPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"
#include "generator/plugins/lua/LuaHelperPlugin.h"
#include "generator/plugins/lua/LuaFieldsPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "generator/plugins/lua/LuaIndexMetaMethodPlugin.h"
#include "generator/plugins/ClassCopyMoveDestroyPlugin.h"
#include "generator/plugins/ClassMetadataPlugin.h"
#include "generator/plugins/CWrappersPlugin.h"
#include "generator/plugins/csharp/DotNetModulePlugin.h"
#include "generator/plugins/csharp/DotNetModuleFunctionsPlugin.h"
#include "generator/plugins/csharp/DotNetHostPlugin.h"

namespace holgen {

Translator::Translator(const TranslatorSettings &translatorSettings) :
    mTranslatorSettings(translatorSettings) {
  // TODO: Users should pick which plugins to run
  AddPlugin<ClassPlugin>();
  AddPlugin<ClassIdFieldPlugin>();
  AddPlugin<ClassFieldPlugin>();
  AddPlugin<CppDestructorPlugin>();
  AddPlugin<LuaFieldsPlugin>();
  AddPlugin<ClassFieldVariantPlugin>();
  AddPlugin<ClassEqualsOperatorPlugin>();
  AddPlugin<ClassFieldGetterPlugin>();
  AddPlugin<ClassFieldSetterPlugin>();
  AddPlugin<LuaFunctionPlugin>();
  AddPlugin<CppFunctionPlugin>();
  AddPlugin<ContainerFieldPlugin>();
  AddPlugin<ManagedClassPlugin>();
  AddPlugin<EnumPlugin>();
  AddPlugin<BitmapPlugin>();
  AddPlugin<BitmapFieldPlugin>();
  AddPlugin<DotNetModulePlugin>();
  AddPlugin<DotNetModuleFunctionsPlugin>();
  AddPlugin<DotNetHostPlugin>();
  AddPlugin<CWrappersPlugin>();
  AddPlugin<JsonConverterPlugin>();
  AddPlugin<JsonPlugin>();
  AddPlugin<JsonParseFilesPlugin>();
  AddPlugin<JsonHelperPlugin>();
  // TODO: lua plugins should run before json so that LuaRegistryData can be initialized via json
  AddPlugin<LuaPlugin>();
  AddPlugin<LuaIndexMetaMethodPlugin>();
  AddPlugin<LuaHelperPlugin>();
  AddPlugin<GlobalPointerPlugin>();
  AddPlugin<FilesystemHelperPlugin>();
  AddPlugin<SingletonPlugin>();
  AddPlugin<ClassCopyMoveDestroyPlugin>();
  AddPlugin<ClassMetadataPlugin>();
}

TranslatedProject Translator::Translate(const ProjectDefinition &project) const {
  TranslatedProject translatedProject(project);
  for (const auto &plugin: mPlugins) {
    plugin(translatedProject);
  }
  return translatedProject;
}
} // namespace holgen
