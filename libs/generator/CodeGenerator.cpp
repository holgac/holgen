#include "CodeGenerator.h"

#include "generators/CppHeaderGenerator.h"
#include "generators/CppSourceGenerator.h"
#include "generators/CppModifiableSourceGenerator.h"
#include "generators/HolgenHeaderGenerator.h"
#include "generators/CMakeGenerator.h"
#include "generators/CSharpWrapperGenerator.h"
#include "generators/CSharpProjectGenerator.h"

namespace holgen {

std::vector<GeneratedContent> CodeGenerator::Generate(const TranslatedProject &translatedProject) {
  mTranslatedProject = &translatedProject;
  std::vector<GeneratedContent> contents;
  CppHeaderGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CppSourceGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CppModifiableSourceGenerator(mGeneratorSettings, translatedProject).Run(contents);
  HolgenHeaderGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CMakeGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CSharpWrapperGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CSharpProjectGenerator(mGeneratorSettings, translatedProject).Run(contents);

  mTranslatedProject = nullptr;
  return contents;
}
} // namespace holgen
