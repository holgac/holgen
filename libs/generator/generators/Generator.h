#pragma once
#include "../TranslatedProject.h"

#include <generator/CodeGenerator.h>

namespace holgen {
class Generator {
public:
  explicit Generator(const GeneratorSettings &generatorSettings,
                     const TranslatedProject &translatedProject) :
      mGeneratorSettings(generatorSettings), mTranslatedProject(translatedProject) {}

  virtual ~Generator() = default;

  virtual void Run(std::vector<GeneratedContent> &contents) = 0;

protected:
  const GeneratorSettings &mGeneratorSettings;
  const TranslatedProject &mTranslatedProject;
};
} // namespace holgen
