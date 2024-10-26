#pragma once
#include "Generator.h"

namespace holgen {
class CSharpWrapperGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) const override;
};
} // namespace holgen
