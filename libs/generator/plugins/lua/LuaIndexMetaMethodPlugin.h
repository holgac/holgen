#pragma once
#include "LuaPluginBase.h"
#include "generator/utils/StringSwitcher.h"

namespace holgen {

class LuaIndexMetaMethodPlugin : public LuaPluginBase {
public:
  using LuaPluginBase::LuaPluginBase;
  void Run() override;

private:
  void Process(Class &cls);
  void GenerateIndexMetaMethodForFields(Class &cls, StringSwitcher &switcher);
  void GenerateMethodCaller(Class &cls, const ClassMethod &method);
  void GenerateIndexMetaMethodForExposedMethods(Class &cls, StringSwitcher &switcher);
  void GenerateIndexMetaMethodForFuncTable(Class &cls, StringSwitcher &switcher);
  void GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  std::string GenerateReadExposedMethodArgsAndGetArgsString(const Class& cls, const ClassMethod &exposedMethod,
                                                            CodeBlock &switchBlock, bool isLuaFunc);
  void GenerateIndexForVariantField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  void GenerateIndexForRegistryData(ClassField &field, CodeBlock &switchBlock);
  void GenerateInstanceGetter(Class &cls, CodeBlock &codeBlock, int index,
                              const std::string &outVarName);
};

} // namespace holgen
