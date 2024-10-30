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
  [[nodiscard]] std::string GenerateFunctionPointerCall(const Class &cls,
                                                        const ClassMethod &method, CodeBlock& preWork, CodeBlock& postWork);
  void ProcessForCSharp(Class &cls);
  CSharpClass &GenerateCSharpClass(Class &cls);
  void GenerateCSharpPointerField(CSharpClass &csCls);
  void GenerateCSharpMethods(const Class &cls, CSharpClass &csCls);
  CSharpMethod &GenerateCSharpAbstractMethod(const Class &cls, CSharpClass &csCls,
                                             const ClassMethod &method);
  void GenerateCSharpMethodCallerMethod(const Class &cls, CSharpClass &csCls,
                                        const ClassMethod &method, const CSharpMethod &csMethod);
  [[nodiscard]] std::string GenerateCSharpMethodCall(const Class &cls, CSharpClass &csCls,
                                                     const ClassMethod &method,
                                                     const CSharpMethod &csMethod);
};

} // namespace holgen
