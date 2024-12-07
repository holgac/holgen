#pragma once
#include "../TranslatorPlugin.h"

namespace holgen {
class LuaPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
protected:
  void GenerateMethodCaller(Class &cls, const ClassMethod &method, const std::string& methodName);
  void GenerateInstanceGetter(Class &cls, CodeBlock &codeBlock, int index,
                              const std::string &outVarName);
  std::string GenerateReadExposedMethodArgsAndGetArgsString(const Class& cls, const ClassMethod &exposedMethod,
                                                            CodeBlock &switchBlock, bool isLuaFunc);
};

} // namespace holgen
