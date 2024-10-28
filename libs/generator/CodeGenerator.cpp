#include "CodeGenerator.h"

#include "generators/CppHeaderGenerator.h"
#include "generators/CppSourceGenerator.h"
#include "generators/CppModifiableSourceGenerator.h"
#include "generators/HolgenHeaderGenerator.h"
#include "generators/CMakeGenerator.h"
#include "generators/CSharpWrapperGenerator.h"
#include "generators/CSharpProjectGenerator.h"
#include "generators/CSharpHolgenMainGenerator.h"
#include "generators/CSharpModuleInterfaceGenerator.h"

namespace holgen {

std::vector<GeneratedContent> CodeGenerator::Generate(const TranslatedProject &translatedProject) {
  mTranslatedProject = &translatedProject;
  // TODO(RELEASE): Move TranslatedProject arg to ctor, and get this from TranslatorSettings
  NamingConvention namingConvention(*mTranslatedProject);
  std::vector<GeneratedContent> contents;
  CppHeaderGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CppSourceGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CppModifiableSourceGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  HolgenHeaderGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CMakeGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CSharpWrapperGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CSharpProjectGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CSharpHolgenMainGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);
  CSharpModuleInterfaceGenerator(mGeneratorSettings, translatedProject, namingConvention).Run(contents);

  mTranslatedProject = nullptr;
  return contents;
}
} // namespace holgen
