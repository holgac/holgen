#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
class LuaFunctionPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void GenerateTableSetter(Class &cls);
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition,
                               bool isFuncTable);
  void ProcessLuaFunction(Class &cls, const FunctionDefinition &functionDefinition,
                          bool isFuncTable);
  void GenerateFunctionSetter(Class &cls, const FunctionDefinition &functionDefinition,
                              ClassField &functionHandle);
  // TODO: arg order should be <cls>, <definitions>, <rest>
  void GenerateFunctionGetFunctionFromSourceTable(const FunctionDefinition &functionDefinition,
                                                  const std::string *sourceTable,
                                                  const std::string &functionHandle,
                                                  bool isFuncTable, ClassMethod &method,
                                                  const std::string &retVal);
  void GenerateFunctionGetGlobalFunction(const FunctionDefinition &functionDefinition,
                                         const std::string &functionHandle, bool isFuncTable,
                                         ClassMethod &method, const std::string &retVal);
  void GenerateFunctionChecker(Class &cls, const FunctionDefinition &functionDefinition,
                               ClassField &functionHandle);
  void GenerateFunctionPushArgs(ClassMethod &method, const FunctionDefinition &functionDefinition);
  void GenerateFunction(Class &cls, const FunctionDefinition &functionDefinition,
                        const std::string *sourceTable, const std::string &functionHandle,
                        bool isFuncTable, bool isStatic);
};
} // namespace holgen
