#pragma once
#include "Generator.h"

namespace holgen {
class LuaSourceGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Generate(GeneratedContent &out, const Class &cls) const;
  void GenerateEnum(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateClassDefinition(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateFields(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateField(CodeBlock &codeBlock, const ClassField &field) const;
  void GenerateMethods(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateMethod(CodeBlock &codeBlock, const Class &cls, const ClassMethod &method) const;
  bool ShouldProcess(const Class &cls) const;
  bool ShouldProcess(const ClassField &field) const;
  bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] std::string ToLuaType(const Type &type) const;
};
} // namespace holgen
