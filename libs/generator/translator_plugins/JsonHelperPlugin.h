#pragma once


#include <map>
#include <set>
#include <string>
#include "TranslatorPlugin.h"

namespace holgen {
  class JsonHelperPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void GenerateHelpers() override;

  private:
    void GenerateJsonHelper(Class &generatedClass);
  };
}