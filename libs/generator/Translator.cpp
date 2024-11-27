#include "Translator.h"
#include "generator/plugins/cpp/ClassEqualsOperatorPlugin.h"
#include "generator/plugins/cpp/ClassFieldGetterPlugin.h"
#include "generator/plugins/cpp/ClassFieldPlugin.h"
#include "generator/plugins/cpp/ClassFieldSetterPlugin.h"
#include "generator/plugins/cpp/ClassFieldVariantPlugin.h"
#include "generator/plugins/cpp/ClassIdFieldPlugin.h"
#include "generator/plugins/cpp/ClassPlugin.h"
#include "generator/plugins/cpp/ContainerFieldPlugin.h"
#include "generator/plugins/cpp/CppDestructorPlugin.h"
#include "generator/plugins/cpp/CppFunctionPlugin.h"
#include "generator/plugins/cpp/ManagedClassPlugin.h"
#include "generator/plugins/cpp/ClassCopyMoveDestroyPlugin.h"
#include "generator/plugins/cpp/ClassMetadataPlugin.h"
#include "generator/plugins/cpp/CWrappersPlugin.h"
#include "generator/plugins/cpp/CompositeIdTypePlugin.h"
#include "generator/plugins/cpp/CompositeIdStructPlugin.h"
#include "generator/plugins/enum/EnumPlugin.h"
#include "generator/plugins/enum/BitmapPlugin.h"
#include "generator/plugins/enum/BitmapFieldPlugin.h"
#include "generator/plugins/helpers/FilesystemHelperPlugin.h"
#include "generator/plugins/helpers/GlobalPointerPlugin.h"
#include "generator/plugins/helpers/SingletonPlugin.h"
#include "generator/plugins/helpers/DeferredDeleterPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/json/JsonHelperPlugin.h"
#include "generator/plugins/json/JsonParsePlugin.h"
#include "generator/plugins/json/JsonDumpPlugin.h"
#include "generator/plugins/json/JsonParseFilesPlugin.h"
#include "generator/plugins/json/JsonDumpFilesPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"
#include "generator/plugins/lua/LuaHelperPlugin.h"
#include "generator/plugins/lua/LuaFieldsPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "generator/plugins/lua/LuaIndexMetaMethodPlugin.h"
#include "generator/plugins/csharp/DotNetModulePlugin.h"
#include "generator/plugins/csharp/DotNetModuleFunctionsPlugin.h"
#include "generator/plugins/csharp/DotNetHostPlugin.h"
#include "generator/plugins/csharp/DotNetModuleInterfacePlugin.h"
#include "generator/plugins/csharp/DotNetWrapperPlugin.h"
#include "generator/plugins/csharp/DotNetClassPlugin.h"
#include "generator/plugins/csharp/DotNetInterfacePlugin.h"
#include "generator/plugins/csharp/DotNetInterfaceClassPlugin.h"
#include "generator/plugins/csharp/DotNetDeferredDeleterPlugin.h"

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
  AddPlugin<ClassFieldGetterPlugin>();
  AddPlugin<ClassFieldSetterPlugin>();
  AddPlugin<LuaFunctionPlugin>();
  AddPlugin<CppFunctionPlugin>();
  AddPlugin<ContainerFieldPlugin>();
  AddPlugin<ManagedClassPlugin>();
  AddPlugin<EnumPlugin>();
  AddPlugin<BitmapPlugin>();
  AddPlugin<BitmapFieldPlugin>();
  AddPlugin<DeferredDeleterPlugin>();
  AddPlugin<CompositeIdTypePlugin>();
  AddPlugin<CompositeIdStructPlugin>();
  AddPlugin<CWrappersPlugin>();
  AddPlugin<DotNetModulePlugin>();
  AddPlugin<DotNetModuleFunctionsPlugin>();
  AddPlugin<DotNetClassPlugin>();
  AddPlugin<DotNetWrapperPlugin>();
  AddPlugin<DotNetInterfacePlugin>();
  AddPlugin<DotNetInterfaceClassPlugin>();
  AddPlugin<DotNetDeferredDeleterPlugin>();
  // Any csharp class that needs initialization needs to be added before DotNetHostPlugin runs
  AddPlugin<DotNetHostPlugin>();
  AddPlugin<DotNetModuleInterfacePlugin>();
  AddPlugin<ClassEqualsOperatorPlugin>();
  AddPlugin<JsonConverterPlugin>();
  AddPlugin<JsonParsePlugin>();
  AddPlugin<JsonDumpPlugin>();
  AddPlugin<JsonParseFilesPlugin>();
  AddPlugin<JsonDumpFilesPlugin>();
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
