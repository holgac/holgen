#pragma once

#include "TranslatorPlugin.h"

namespace holgen {

class LuaPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void EnrichClasses() override;
private:
  void EnrichClass(Class &generatedClass, const StructDefinition& structDefinition);
  void EnrichClass(Class &generatedClass, const EnumDefinition& enumDefinition);
  void CreateIndexMetaMethod(CodeBlock& codeBlock, Class& generatedClass);
  void CreateNewIndexMetaMethod(CodeBlock& codeBlock, Class& generatedClass);
  void AddLuaInstanceGetter(CodeBlock& codeBlock, Class& cls, int idx);
  };

}
