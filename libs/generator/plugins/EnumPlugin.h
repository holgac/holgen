#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class EnumPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  private:
    void GenerateCommon(Class& cls);
    void GenerateOperators(Class& cls);
    void GenerateEntries(Class& cls);
    void GenerateFromString(Class& cls);
    void GenerateToString(Class& cls);
    void GenerateGetEntries(Class& cls, bool forValues);
  };
}
