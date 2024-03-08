#pragma once

#include "TranslatorPlugin.h"

namespace holgen {

  class GeneratorFilesystemHelper : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void GenerateHelpers() override;
  };

}
