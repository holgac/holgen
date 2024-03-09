#pragma once


#include <map>
#include <set>
#include <string>
#include "TranslatorPlugin.h"

namespace holgen {
  class JsonPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void EnrichClasses() override;
    void GenerateHelpers() override;

  private:
    void EnrichClass(Class &cls, const StructDefinition& structDefinition);
    void EnrichClass(Class &cls,const EnumDefinition& enumDefinition);
    void GenerateParseJson(Class &cls);
    void GenerateParseFiles(Class &cls);
    void GenerateConverter(Class& cls);
    void GenerateParseJsonForField(Class& cls, ClassMethod& parseFunc, const StructDefinition& structDefinition, const FieldDefinition& fieldDefinition);
    // void GenerateParseJsonForField(Class& cls, ClassMethod& parseFunc, const EnumDefinition& enumDefinition, const FieldDefinition& fieldDefinition);
    void GenerateJsonHelper(Class &generatedClass);
  };
}