#pragma once
#include "Generator.h"

namespace holgen {
class CSharpHolgenMainGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;
};
} // namespace holgen
