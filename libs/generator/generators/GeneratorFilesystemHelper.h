#pragma once

#include "Generator.h"

namespace holgen {

class GeneratorFilesystemHelper : public Generator {
public:
  using Generator::Generator;
  void GenerateHelpers() override;
};

}
