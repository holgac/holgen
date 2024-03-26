#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class LuaPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void EnrichClass(Class &generatedClass, const StructDefinition& structDefinition);
  void AddPushGlobalToLua(Class &generatedClass, const StructDefinition& structDefinition);
  void AddPushToLua(Class &generatedClass, const StructDefinition& structDefinition);
  void AddReadFromLua(Class &generatedClass, const StructDefinition& structDefinition);
  void EnrichClass(Class &generatedClass, const EnumDefinition& enumDefinition);
  void CreatePushIndexMetaMethod(Class& cls);
  void CreatePushNewIndexMetaMethod(Class& cls);
  };

}
