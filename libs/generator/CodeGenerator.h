#pragma once

#include <string>
#include "Translator.h"
#include "GeneratorSettings.h"

namespace holgen {

class CodeGenerator {

public:
  explicit CodeGenerator(const GeneratorSettings &generatorSettings) :
      mGeneratorSettings(generatorSettings) {}

  std::vector<GeneratedContent> Generate(const TranslatedProject &translatedProject);
  GeneratorSettings mGeneratorSettings;
  const TranslatedProject *mTranslatedProject = nullptr;
};
} // namespace holgen
