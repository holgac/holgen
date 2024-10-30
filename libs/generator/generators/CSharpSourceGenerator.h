#pragma once
#include "Generator.h"

namespace holgen {
class CSharpSourceGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Process(GeneratedContent &out, const CSharpClass &csCls) const;
  void GenerateClass(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateUsingDirectives(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateClassNamespace(CodeBlock &codeBlock) const;
  void GenerateClassBody(CodeBlock &codeBlock, const CSharpClass &csCls) const;
  void GenerateClassBody(CodeBlock &codeBlock, const CSharpClass &csCls,
                         CSharpVisibility visibility) const;
  void GenerateConstructors(CodeBlock &codeBlock, const CSharpClass &csCls,
                            CSharpVisibility visibility) const;
  void GenerateConstructor(CodeBlock &codeBlock, const CSharpClass &csCls,
                           const CSharpConstructor &ctor) const;
  void GenerateDelegates(CodeBlock &codeBlock, const CSharpClass &csCls,
                         CSharpVisibility visibility) const;
  void GenerateDelegate(CodeBlock &codeBlock, const CSharpClass &csCls,
                        const CSharpMethod &method) const;
  void GenerateMethods(CodeBlock &codeBlock, const CSharpClass &csCls,
                       CSharpVisibility visibility) const;
  void GenerateMethod(CodeBlock &codeBlock, const CSharpClass &csCls,
                      const CSharpMethod &method) const;
  void GenerateFields(CodeBlock &codeBlock, const CSharpClass &csCls,
                      CSharpVisibility visibility) const;
  void GenerateField(CodeBlock &codeBlock, const CSharpClass &csCls,
                     const CSharpClassField &field) const;
  void GenerateInnerClasses(CodeBlock &codeBlock, const CSharpClass &csCls,
                            CSharpVisibility visibility) const;
  void GenerateFieldAccessor(CodeBlock &codeBlock, const CSharpMethodBase &method,
                        const CSharpClassField &field, const std::string &name) const;

  [[nodiscard]] std::string GenerateArgumentsInSignature(const CSharpMethodBase &method) const;
  [[nodiscard]] std::string GenerateAttributes(const std::list<std::string> &attributes) const;
  void GenerateAttributes(CodeBlock &codeBlock, const std::list<std::string> &attributes) const;
};
} // namespace holgen
