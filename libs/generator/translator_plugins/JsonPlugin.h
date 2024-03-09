#pragma once


#include <map>
#include <set>
#include <string>
#include "TranslatorPlugin.h"

namespace holgen {
  class JsonPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;

  private:
    void EnrichClass(Class &cls, const StructDefinition& structDefinition);
    void EnrichClass(Class &cls,const EnumDefinition& enumDefinition);
    void GenerateParseJson(Class &cls);
    void GenerateParseJsonForField(Class& cls, ClassMethod& parseFunc, const StructDefinition& structDefinition, const FieldDefinition& fieldDefinition);
  };
}