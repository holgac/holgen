#pragma once
#include "Generator.h"

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
  bool GenerateMethods(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  bool GenerateMethodDelegates(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls,
                              const ClassMethod &method) const;
  bool GenerateMethodPointers(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethodPointer(CodeBlock &codeBlock, const ClassMethod &method) const;

  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const;
  [[nodiscard]] std::string ConstructInitializerArguments(const Class &cls) const;
  [[nodiscard]] std::string ConstructMethodSignatureArguments(const ClassMethod &method) const;
  [[nodiscard]] std::string ConstructMethodArguments(const ClassMethod &method) const;
};
} // namespace holgen
