#pragma once


#include <map>
#include <set>
#include <string>
#include "parser/Parser.h"
#include "Translator.h"

namespace holgen {
  class GeneratorJson {
  public:
    GeneratorJson(const ProjectDefinition &projectDefinition, TranslatedProject &translatedProject);
    void EnrichClasses();
    void GenerateHelpers();

  private:
    void GenerateParseJson(Class &cls);
    void GenerateConverter(Class& cls);
    void GenerateParseJsonForField(Class& cls, ClassMethod& parseFunc, const StructDefinition& structDefinition, const FieldDefinition& fieldDefinition);
    static void GenerateJsonHelper(Class &generatedClass);
    const ProjectDefinition &mProjectDefinition;
    TranslatedProject &mTranslatedProject;
    std::map<std::string, size_t> mStructDefinitions;
    std::map<std::string, size_t> mClasses;
  };
}