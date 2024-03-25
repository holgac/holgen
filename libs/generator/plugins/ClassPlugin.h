#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ClassPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  };
}
