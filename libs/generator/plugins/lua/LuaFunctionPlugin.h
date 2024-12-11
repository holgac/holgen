#pragma once

#include "../FunctionPluginBase.h"

namespace holgen {
class LuaFunctionPlugin : public FunctionPluginBase {
public:
  using FunctionPluginBase::FunctionPluginBase;
  void Run() override;

private:
  [[nodiscard]] bool IsLuaPublisher(const Class &cls) const;
  void ProcessLuaPublisher(Class &cls) const;
  void GeneratePublisherRegisterSubscriber(Class& cls) const;
  void GeneratePublisherUnregisterSubscriber(Class& cls) const;
  void GeneratePublisherUnregisterSubscriberByName(Class& cls) const;
  void GeneratePublisherClearSubscribers(Class& cls) const;
  void GenerateTableSetter(Class &cls) const;
  void GenerateTableGetter(Class &cls) const;
  void ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition,
                               bool isFuncTable) const;
  void ProcessLuaFunction(Class &cls, const FunctionDefinition &functionDefinition,
                          bool isFuncTable) const;
  void GenerateFunctionChecks(CodeBlock &codeBlock, const std::string &functionName,
                              const std::string &functionHandle, bool throwOnFailure,
                              bool isFuncTable, bool isStatic, bool isPublisher) const;
  void GenerateFunctionSetter(Class &cls, const FunctionDefinition &functionDefinition,
                              ClassField &functionHandle) const;
  // TODO: arg order should be <cls>, <definitions>, <rest>
  void GenerateFunctionGetFunctionFromSourceTable(const std::string &functionName,
                                                  bool throwOnFailure,
                                                  const std::string *sourceTable,
                                                  const std::string &functionHandle,
                                                  bool isFuncTable, ClassMethod &method,
                                                  const std::string &retVal) const;
  void GenerateFunctionGetGlobalFunction(const Class &cls, const std::string &functionName,
                                         bool throwOnFailure, const std::string &functionHandle,
                                         bool isFuncTable, ClassMethod &method,
                                         const std::string &retVal) const;
  void GenerateFunctionChecker(Class &cls, const FunctionDefinition &functionDefinition,
                               ClassField &functionHandle) const;
  void GenerateFunctionPushArgs(ClassMethod &method,
                                const FunctionDefinition &functionDefinition) const;
  void GenerateFunction(Class &cls, const FunctionDefinition &functionDefinition,
                        const std::string *sourceTable, const std::string &functionHandle,
                        bool isFuncTable, bool isStatic) const;
};
} // namespace holgen
