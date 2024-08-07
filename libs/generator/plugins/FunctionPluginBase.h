#pragma once
#include "TranslatorPlugin.h"
#include "generator/TranslatedProject.h"

namespace holgen {
class FunctionPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  ClassMethod NewFunction(Class &cls, const FunctionDefinition &functionDefinition);

private:
  void ProcessFunctionArgument(Class &cls, ClassMethod &method,
                               const FunctionArgumentDefinition &funcArg);
};
} // namespace holgen
