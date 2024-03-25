#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ClassFieldPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  };
}
