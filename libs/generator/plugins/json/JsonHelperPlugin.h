#pragma once


#include <map>
#include <set>
#include <string>
#include "generator/plugins/TranslatorPlugin.h"

namespace holgen {
  class JsonHelperPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;
  };
}