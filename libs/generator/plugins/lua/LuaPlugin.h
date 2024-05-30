#pragma once

#include "holgen.h"
#include "../TranslatorPlugin.h"

namespace holgen {

class LuaPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GeneratePushGlobalToLua(Class &cls);
  void GeneratePushToLua(Class &cls);
  void GenerateReadFromLua(Class &cls);
  void GenerateIndexMetaMethod(Class &cls);
  void GenerateNewIndexMetaMethod(Class &cls);
  void GenerateCreateLuaMetatable(Class &cls);
  void GeneratePushEnumToLua(Class &cls);
  bool ShouldEmbedPointer(Class &cls);
  void GenerateIndexForField(Class &cls, ClassField &field, CodeBlock &switchBlock) const;
  void GenerateReadEnumFromLuaBody(Class &cls, ClassMethod &method) const;
  void GenerateReadStructFromLuaBody(Class &cls, ClassMethod &method);
};

}
