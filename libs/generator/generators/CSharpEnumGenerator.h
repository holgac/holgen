#pragma once
#include "Generator.h"

namespace holgen {
class CSharpEnumGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;

private:
  void Process(GeneratedContent &out, const Class &cls, const ClassEnum& clsEnum) const;

private:
};
} // namespace holgen
