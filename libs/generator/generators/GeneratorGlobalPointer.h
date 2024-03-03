#pragma once

#include "Generator.h"

namespace holgen {
  // This is like a singleton, but the user is expected to handle object deletion
  class GeneratorGlobalPointer : public Generator {
  public:
    using Generator::Generator;

    void GenerateHelpers() override;

  };
}
