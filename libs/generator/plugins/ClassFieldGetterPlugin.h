#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ClassFieldGetterPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  };
}
