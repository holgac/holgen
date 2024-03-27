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
  private:
    void GenerateParseJsonForSingleElemContainer(Class &cls, const std::string &container);
    void GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container);
    void GenerateBaseParse(Class &cls);
    void GenerateParseSingleElem(Class &cls);
    void GenerateParseSingleElem(
        Class &cls, const std::string &type, const std::string &validator, const std::string &getter);
  };
}