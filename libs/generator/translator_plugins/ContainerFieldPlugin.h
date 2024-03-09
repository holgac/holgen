#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ContainerFieldPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  };
}
