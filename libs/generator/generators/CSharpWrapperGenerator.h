#pragma once
#include "Generator.h"

#include "generator/lang/CSharpHelper.h"

namespace holgen {
class CSharpWrapperGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateClassBody(CodeBlock &codeBlock, const Class &cls) const;
  bool GenerateFields(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateInitializer(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateInitializerDelegate(CodeBlock &codeBlock, const Class &cls) const;
  bool GenerateConstructors(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateEmptyConstructor(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateConstructor(CodeBlock &codeBlock, const Class &cls,
                           const ClassConstructor &ctor) const;
  bool GenerateMethods(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  bool GenerateMethodDelegates(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls,
                              const ClassMethod &method) const;
  bool GenerateMethodPointers(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethodPointer(CodeBlock &codeBlock, const ClassMethod &method) const;

  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const ClassConstructor &ctor) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const;
  [[nodiscard]] std::string ConstructInitializerArguments(const Class &cls) const;
  [[nodiscard]] std::string ConstructMethodSignatureArguments(const Class &cls,
                                                              const MethodBase &method,
                                                              InteropType interopType,
                                                              bool addThisArgument) const;
  [[nodiscard]] std::string ConstructMethodArguments(const Class &cls, const MethodBase &method,
                                                     InteropType interopType,
                                                     bool addThisArgument) const;
};
} // namespace holgen
