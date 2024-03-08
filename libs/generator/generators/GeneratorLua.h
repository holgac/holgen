#pragma once

#include "Generator.h"

namespace holgen {

class GeneratorLua : public Generator {
public:
  using Generator::Generator;
  void EnrichClasses() override;
  void GenerateHelpers() override;
private:
  void EnrichClass(Class &generatedClass, const StructDefinition& structDefinition);
  void EnrichClass(Class &generatedClass, const EnumDefinition& enumDefinition);
  void GenerateLuaHelper(Class &generatedClass);
  void GenerateLuaHelperPush(Class &generatedClass);
  void GenerateLuaHelperRead(Class &generatedClass);
  void CreateIndexMetaMethod(CodeBlock& codeBlock, Class& generatedClass);
  void CreateNewIndexMetaMethod(CodeBlock& codeBlock, Class& generatedClass);
  void AddLuaInstanceGetter(CodeBlock& codeBlock, Class& cls, int idx);
  };

}
