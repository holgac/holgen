#pragma once
#include "Generator.h"

namespace holgen {
class CMakeGenerator : public Generator {
public:
  using Generator::Generator;
  void Run(std::vector<GeneratedContent> &contents) override;
};
} // namespace holgen
