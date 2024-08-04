#pragma once

#include "LuaPluginBase.h"
#include "holgen.h"

namespace holgen {
class StringSwitcher;

class LuaPlugin : public LuaPluginBase {
public:
  using LuaPluginBase::LuaPluginBase;
  void Run() override;

private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
  void GeneratePushGlobalToLua(Class &cls);
  void GeneratePushToLua(Class &cls);
  void GeneratePushMirrorStructToLua(Class &cls);
  void GenerateReadProxyObjectFromLua(Class &cls);
  void GenerateReadMirrorObjectFromLua(Class &cls);
  void GenerateNewIndexMetaMethod(Class &cls);
  void GenerateCreateLuaMetatable(Class &cls);
  void GeneratePushEnumToLua(Class &cls);
  bool ShouldEmbedPointer(Class &cls);
  void GenerateReadEnumFromLuaBody(Class &cls, ClassMethod &method);
  void GenerateReadProxyStructFromLuaBody(Class &cls, ClassMethod &method);
  void GenerateReadMirrorStructFromLuaBody(Class &cls, ClassMethod &method);
};

} // namespace holgen
