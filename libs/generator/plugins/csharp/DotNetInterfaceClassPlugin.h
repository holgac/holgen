#pragma once

#include "../FunctionPluginBase.h"

namespace holgen {

class DotNetInterfaceClassPlugin : public FunctionPluginBase {
public:
  using FunctionPluginBase::FunctionPluginBase;
  void Run() override;

private:
  [[nodiscard]] bool ShouldProcess(const Class &cls);
  void Process(Class &cls);
  void ProcessForCpp(Class &cls);
  void GenerateConstructorsForCpp(Class &cls);
  void GenerateFunctionForCpp(Class &cls, const FunctionDefinition &functionDefinition);
  void GenerateFunctionPointerForCpp(Class &cls, const FunctionDefinition &functionDefinition);
  ClassMethod GenerateFunction(Class &cls, const FunctionDefinition &functionDefinition,
                               bool convertArguments, bool addThisArgument);
  [[nodiscard]] std::string GenerateFunctionPointerCall(const Class &cls, const ClassMethod &method,
                                                        CodeBlock &preWork);
  void GenerateReturnStatement(ClassMethod &method, const std::string &assignedValue);
  [[nodiscard]] std::string ConvertBasicStatementForReturn(const std::string &statement,
                                                           const Type &type);
  void ProcessForCSharp(Class &cls);
  CSharpClass &GenerateCSharpClass(Class &cls);
  void GenerateCSharpPointerField(CSharpClass &csCls);
  void GenerateCSharpMethods(const Class &cls, CSharpClass &csCls);
};

} // namespace holgen
