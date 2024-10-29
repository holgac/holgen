#pragma once
#include "CSharpGeneratorBase.h"

namespace holgen {
class CSharpWrapperGenerator : public CSharpGeneratorBase {
public:
  using CSharpGeneratorBase::CSharpGeneratorBase;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateClassBody(CodeBlock &codeBlock, const Class &cls) const;
  bool GenerateFields(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateInitializer(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateInitializerDelegate(CodeBlock &codeBlock, const Class &cls) const;
  bool GenerateConstructors(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateProxyConstructor(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateEmptyConstructor(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateConstructor(CodeBlock &codeBlock, const Class &cls,
                           const ClassConstructor &ctor) const;
  bool GenerateMethods(CodeBlock &codeBlock, const Class &cls) const;
  bool GenerateMethodDelegates(CodeBlock &codeBlock, const Class &cls) const;
  bool GenerateMethodPointers(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethodPointer(CodeBlock &codeBlock, const ClassMethod &method) const;

  [[nodiscard]] bool ShouldProcess(const Class &cls) const;
  [[nodiscard]] bool ShouldProcess(const CFunction &func) const;
  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const ClassConstructor &ctor) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const;
  [[nodiscard]] std::string ConstructInitializerArguments(const Class &cls) const;
};
} // namespace holgen
