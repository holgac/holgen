#pragma once
#include "LuaPluginBase.h"
#include "generator/StringSwitcher.h"

namespace holgen {

class LuaIndexMetaMethodPlugin : public LuaPluginBase {
public:
  using LuaPluginBase::LuaPluginBase;
  void Run() override;

private:
  void Process(Class &cls);
  void GenerateIndexMetaMethodForFields(Class &cls, StringSwitcher &switcher);
  void GenerateIndexMetaMethodForExposedMethods(Class &cls, StringSwitcher &switcher);
  void GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  std::string GenerateReadExposedMethodArgsAndGetArgsString(ClassMethod &exposedMethod,
                                                            CodeBlock &switchBlock, bool isLuaFunc);
  void GenerateIndexForVariantField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  void GenerateIndexForRegistryData(ClassField &field, CodeBlock &switchBlock);
};

} // namespace holgen
