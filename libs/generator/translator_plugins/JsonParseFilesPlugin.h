#pragma once


#include <map>
#include <set>
#include <string>
#include "TranslatorPlugin.h"

namespace holgen {
  class JsonParseFilesPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void EnrichClasses() override;

  private:
    void GenerateParseFiles(Class &cls);
  };
}