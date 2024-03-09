#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  // This is like a singleton, but the user is expected to handle object deletion
  class GlobalPointerPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;

    void GenerateHelpers() override;

  };
}
