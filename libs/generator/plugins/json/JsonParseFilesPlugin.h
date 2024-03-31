#pragma once


#include <map>
#include <set>
#include <string>
#include "../TranslatorPlugin.h"

namespace holgen {
  class JsonParseFilesPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;

  private:
    void GenerateConverterPopulators(Class &cls, ClassMethod &method);
    void GenerateParseFiles(Class &cls);
    void GenerateFilesByName(ClassMethod &method);
  };
}