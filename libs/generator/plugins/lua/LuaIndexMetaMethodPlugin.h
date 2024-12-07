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
  void GenerateIndexMetaMethodForExposedMethods(Class &cls, StringSwitcher &switcher);
  void GenerateIndexMetaMethodForFuncTable(Class &cls, StringSwitcher &switcher);
  void GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  void GenerateIndexForVariantField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  void GenerateIndexForRegistryData(ClassField &field, CodeBlock &switchBlock);
  bool ShouldGenerateWrapper(const ClassMethod& method) const;
};

} // namespace holgen
