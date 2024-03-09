#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ManagedClassPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void EnrichClasses() override;
  };
}
