#pragma once

#include "TranslatorPlugin.h"

namespace holgen {

  class FilesystemHelperPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;
  };

}
