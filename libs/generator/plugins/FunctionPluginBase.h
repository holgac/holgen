#pragma once
#include "TranslatorPlugin.h"
#include "generator/TranslatedProject.h"

namespace holgen {
class FunctionPluginBase : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;

protected:
  ClassMethod NewFunction(Class &cls, const FunctionDefinition &functionDefinition) const;
  ClassConstructor NewConstructor(Class &cls, const FunctionDefinition &functionDefinition) const;

private:
  void ProcessFunctionArgument(MethodBase &method, const FunctionArgumentDefinition &funcArg) const;
  void ProcessHashFunction(Class &cls, ClassMethod &method) const;
  void ProcessToStringFunction(Class &cls, ClassMethod &method) const;
  void ProcessMethodVisibility(MethodBase &method,
                               const FunctionDefinition &functionDefinition) const;
  bool CanExposeInCSharp( const FunctionDefinition &functionDefinition) const;
};
} // namespace holgen
