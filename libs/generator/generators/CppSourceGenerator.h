#pragma once
#include "Generator.h"

namespace holgen {
class CppSourceGenerator  : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;
private:
  void Generate(GeneratedContent &out, const Class& cls) const;
  [[nodiscard]] CodeBlock GenerateConstructorsForSource(const Class &cls) const;
  [[nodiscard]] CodeBlock GenerateFieldsForSource(const Class &cls) const;
  [[nodiscard]] CodeBlock GenerateMethodsForSource(const Class &cls) const;
  void GenerateCFunctionsForSource(CodeBlock &codeBlock, const Class &cls) const;
};
} // namespace holgen
