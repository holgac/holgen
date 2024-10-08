#pragma once
#include "TranslatorPlugin.h"
#include "generator/TranslatedProject.h"

namespace holgen {
class FunctionPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  ClassMethod NewFunction(Class &cls, const FunctionDefinition &functionDefinition);
  ClassConstructor NewConstructor(Class &cls, const FunctionDefinition &functionDefinition);

private:
  void ProcessFunctionArgument(ClassMethodBase &method, const FunctionArgumentDefinition &funcArg);
  void ProcessHashFunction(Class &cls, ClassMethod &method);
  void ProcessToStringFunction(Class &cls, ClassMethod &method);
  void ProcessMethodVisibility(ClassMethodBase &method,
                               const FunctionDefinition &functionDefinition);
};
} // namespace holgen
