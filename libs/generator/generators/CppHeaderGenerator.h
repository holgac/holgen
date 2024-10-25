#pragma once
#include "Generator.h"

namespace holgen {
class CppHeaderGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateClassDefinition(const Class &cls, CodeBlock &codeBlock) const;
  std::string GenerateClassDeclaration(const Class &cls) const;
  void GenerateForVisibility(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
  void GenerateUsingsForHeader(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateNestedEnumsForHeader(CodeBlock &codeBlock, const Class &cls,
                                    Visibility visibility) const;
  void GenerateConstructorsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility,
                                     bool isInsideClass) const;
  void GenerateMethodsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility,
                                bool isInsideClass) const;
  void GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls,
                                 Visibility visibility) const;
  void GenerateCFunctionsForHeader(CodeBlock &codeBlock, const Class &cls) const;
};
} // namespace holgen
