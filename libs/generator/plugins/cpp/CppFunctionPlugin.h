#pragma once

#include "../FunctionPluginBase.h"

namespace holgen {
class CppFunctionPlugin : public FunctionPluginBase {
public:
  using FunctionPluginBase::FunctionPluginBase;
  void Run() override;

private:
  void AddCppFunction(Class &cls, const FunctionDefinition &functionDefinition);
  void ProcessConstructorAttribute(Class &cls, ClassMethod &method);
  void AddCppConstructor(Class &cls, const FunctionDefinition &functionDefinition);
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition);
};
} // namespace holgen
