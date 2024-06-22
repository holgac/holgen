#pragma once

#include "../TranslatorPlugin.h"
#include "holgen.h"

namespace holgen {
class StringSwitcher;

class LuaPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GeneratePushGlobalToLua(Class &cls);
  void GeneratePushToLua(Class &cls);
  void GeneratePushMirrorStructToLua(Class &cls);
  void GenerateReadProxyObjectFromLua(Class &cls);
  void GenerateReadMirrorObjectFromLua(Class &cls);
  void GenerateIndexMetaMethod(Class &cls);
  void GenerateNewIndexMetaMethod(Class &cls);
  void GenerateCreateLuaMetatable(Class &cls);
  void GeneratePushEnumToLua(Class &cls);
  bool ShouldEmbedPointer(Class &cls);
  void GenerateIndexForVariantField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  void GenerateIndexForRegistryData(ClassField &field, CodeBlock &switchBlock);
  void GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock);
  void GenerateReadEnumFromLuaBody(Class &cls, ClassMethod &method);
  void GenerateReadProxyStructFromLuaBody(Class &cls, ClassMethod &method);
  void GenerateReadMirrorStructFromLuaBody(Class &cls, ClassMethod &method);
  void GenerateIndexMetaMethodForFields(Class &cls, StringSwitcher &switcher);
  void GenerateIndexMetaMethodForExposedMethods(Class &cls, StringSwitcher &switcher);
  std::string GenerateReadExposedMethodArgsAndGetArgsString(ClassMethod &exposedMethod,
                                                            CodeBlock &switchBlock, bool isLuaFunc);
};

} // namespace holgen
