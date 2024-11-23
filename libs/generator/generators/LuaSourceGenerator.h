#pragma once
#include "LuaGeneratorBase.h"

namespace holgen {
class LuaSourceGenerator : public LuaGeneratorBase {
public:
  using LuaGeneratorBase::LuaGeneratorBase;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateEnum(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateClassDefinition(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateFields(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateField(CodeBlock &codeBlock, const ClassField &field) const;
  void GenerateMethodAsField(CodeBlock &codeBlock, const Class &cls,
                             const ClassMethod &method) const;
  void GenerateMethods(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  [[nodiscard]] bool ShouldProcess(const Class &cls) const override;
  [[nodiscard]] bool ShouldProcess(const ClassField &field) const;
  [[nodiscard]] std::string ToTypedFunctionArguments(const Class &cls,
                                                     const ClassMethod &method) const;
  [[nodiscard]] std::string ToFunctionSignature(const Class &cls, const ClassMethod &method) const;
};
} // namespace holgen
