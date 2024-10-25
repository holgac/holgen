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
  std::string GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
                                        bool isInHeader, bool isInsideClass) const;
  std::string GenerateFunctionSignature(const Class &cls, const ClassConstructor &ctor,
                                        bool isInHeader, bool isInsideClass) const;
  std::string GenerateFunctionSignature(const CFunction &func, bool isInHeader) const;
};
} // namespace holgen
