#pragma once


#include <map>
#include <set>
#include <string>
#include "TranslatorPlugin.h"

namespace holgen {
  class JsonConverterPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;
  };
}