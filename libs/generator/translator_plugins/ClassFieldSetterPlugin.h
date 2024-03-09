#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ClassFieldSetterPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void EnrichClasses() override;
  };
}
