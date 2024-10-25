#pragma once
#include "Generator.h"

namespace holgen {
class CppModifiableSourceGenerator  : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;
private:
  void Generate(GeneratedContent &out, const Class& cls) const;
};
} // namespace holgen
