#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class EnumPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void EnrichClasses() override;
  };
}
