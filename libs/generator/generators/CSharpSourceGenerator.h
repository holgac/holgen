#pragma once
#include "Generator.h"

namespace holgen {
class CSharpSourceGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Process(GeneratedContent &out, const CSharpClass &csCls) const;
  void GenerateUsingDirectives(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateClassNamespace(CodeBlock &codeBlock) const;
  void GenerateClassBody(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateDelegates(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateDelegate(CodeBlock &codeBlock, const CSharpClass &csCls,
                        const CSharpMethod &method) const;
  void GenerateMethods(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateMethod(CodeBlock &codeBlock, const CSharpClass &csCls,
                      const CSharpMethod &method) const;

  [[nodiscard]] std::string GenerateArgumentsInSignature(const CSharpMethod &method) const;
};
} // namespace holgen
