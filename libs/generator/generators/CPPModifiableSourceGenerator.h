#pragma once
#include "Generator.h"

namespace holgen {
class CPPModifiableSourceGenerator  : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) override;
private:
  void Generate(GeneratedContent &out, const Class& cls);
};
} // namespace holgen
