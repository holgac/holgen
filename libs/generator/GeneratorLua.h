#pragma once

#include "parser/Parser.h"
#include "Translator.h"

namespace holgen {

class GeneratorLua {
public:
  GeneratorLua(const ProjectDefinition &projectDefinition, TranslatedProject &translatedProject);
  void EnrichClasses();
  void GenerateHelpers();
private:
  void GenerateLuaHelper(Class &generatedClass);
  void GenerateLuaHelperPush(Class &generatedClass);
  void GenerateLuaHelperRead(Class &generatedClass);
  void CreateIndexMetaMethod(CodeBlock& codeBlock, Class& generatedClass);
  void CreateNewIndexMetaMethod(CodeBlock& codeBlock, Class& generatedClass);
  const ProjectDefinition &mProjectDefinition;
  TranslatedProject &mTranslatedProject;
};

}
