#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class EnumPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  private:
    void GenerateCommon(Class& generatedClass, const EnumDefinition& enumDefinition);
    void GenerateOperators(Class& generatedClass);
    void GenerateGetEntries(Class& generatedClass, bool forValues);
  };
}
