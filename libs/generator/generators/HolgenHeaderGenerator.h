#pragma once
#include "Generator.h"

namespace holgen {
class HolgenHeaderGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void GenerateErrorCheckMacros(CodeBlock &codeBlock) const;
  void GenerateExportMacro(CodeBlock &codeBlock) const;
  void GenerateEnumConcept(CodeBlock &codeBlock) const;
};
} // namespace holgen
