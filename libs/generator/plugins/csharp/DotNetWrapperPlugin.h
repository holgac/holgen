#pragma once

#include "../TranslatorPlugin.h"
#include "DotNetPluginBase.h"

namespace holgen {
class DotNetWrapperPlugin : public DotNetPluginBase {
public:
  using DotNetPluginBase::DotNetPluginBase;
  void Run() override;

private:
  void Process(Class &cls, CSharpClass& csCls) const;
  void ProcessConstructors(const Class &cls, CSharpClass &csCls) const;
  void ProcessMethods(const Class &cls, CSharpClass &csCls) const;
  void ProcessMethodPointers(const Class &cls, CSharpClass &csCls) const;
  void ProcessProxy(const Class &cls, CSharpClass &csCls) const;
  void ProcessMirror(const Class &cls, CSharpClass &csCls) const;
  void ProcessInitializer(const Class &cls, CSharpClass &csCls) const;
  void ProcessInitializerArguments(const Class &cls, const CSharpClass &csCls,
                                   CSharpMethod &csMethod) const;
  [[nodiscard]] bool ShouldProcess(const Class &cls) const;
  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const ClassConstructor &ctor) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const;
  [[nodiscard]] bool IsStaticClass(const Class &cls) const;
  void GenerateConstructorWrapperCall(CodeBlock &codeBlock, const Class &cls,
                                      const CSharpClass &csCls, const ClassMethod &method,
                                      const CSharpMethodBase &csMethod) const;
  void GenerateWrapperCall(CodeBlock &codeBlock, const CSharpClass &cls, const ClassMethod &method,
                           const CSharpMethodBase &csMethod, bool addThisArgument) const;
  void GenerateWrapperCallReturningVoid(CodeBlock &codeBlock, const CSharpClass &cls, const ClassMethod &method,
                           const CSharpMethodBase &csMethod, bool addThisArgument) const;
  void GenerateWrapperCallReturningArray(CodeBlock &codeBlock, const CSharpClass &cls, const ClassMethod &method,
                           const CSharpMethodBase &csMethod, bool addThisArgument) const;
  void GenerateWrapperCallReturningValue(CodeBlock &codeBlock, const CSharpClass &cls, const ClassMethod &method,
                           const CSharpMethodBase &csMethod, bool addThisArgument) const;
  void GenerateWrapperCallReturningClass(CodeBlock &codeBlock, const CSharpClass &cls, const ClassMethod &method,
                           const CSharpMethodBase &csMethod, bool addThisArgument, const Class& returnType) const;
  [[nodiscard]] std::string ConstructWrapperCall(const CSharpClass &cls, const MethodBase &method,
                                                 const CSharpMethodBase &csMethod,
                                                 const std::string &methodName,
                                                 bool addThisArgument, bool hasSizeArg,
                                                 bool hasDeleterArg) const;
  [[nodiscard]] std::string ConstructMethodArguments(const CSharpClass &cls,
                                                     const MethodBase &method,
                                                     const CSharpMethodBase &csMethod,
                                                     InteropType interopType, bool addThisArgument,
                                                     bool hasSizeArg, bool hasDeleterArg) const;
};
} // namespace holgen
