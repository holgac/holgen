#pragma once


#include <map>
#include <set>
#include <string>
#include "Generator.h"

namespace holgen {
  class GeneratorJson : public Generator {
  public:
    using Generator::Generator;
    void EnrichClasses() override;
    void GenerateHelpers() override;

  private:
    void GenerateParseJson(Class &cls);
    void GenerateParseFiles(Class &cls);
    void GenerateConverter(Class& cls);
    void GenerateParseJsonForField(Class& cls, ClassMethod& parseFunc, const StructDefinition& structDefinition, const FieldDefinition& fieldDefinition);
    void GenerateJsonHelper(Class &generatedClass);
  };
}