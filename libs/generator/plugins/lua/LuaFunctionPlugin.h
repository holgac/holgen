#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class LuaFunctionPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition);
  void ProcessLuaFunction(Class &cls, const FunctionDefinition &functionDefinition);
  void GenerateFunctionSetter(Class &cls, const FunctionDefinition &functionDefinition, ClassField &functionHandle);
  void GenerateFunctionChecker(Class &cls, const FunctionDefinition &functionDefinition, ClassField &functionHandle);
  void GenerateFunctionPushArgs(ClassMethod &method, const FunctionDefinition &functionDefinition);
  void GenerateFunction(
      Class &cls, const FunctionDefinition &functionDefinition, ClassField &functionHandle);
};
}
