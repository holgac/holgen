#pragma once
#include "TranslatorPlugin.h"
#include "generator/TranslatedProject.h"

namespace holgen {
class FunctionPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  ClassMethod NewFunction(Class &cls, const FunctionDefinition &functionDefinition);

private:
  void ProcessFunctionArgument(ClassMethod &method, const FunctionArgumentDefinition &funcArg);
  void ProcessHashFunction(Class &cls, ClassMethod &method);
  void ProcessToStringFunction(Class &cls, ClassMethod &method);
};
} // namespace holgen
