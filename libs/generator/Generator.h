#pragma once

#include "Translator.h"
#include <string>


namespace holgen {
  enum class FileType {
    CppHeader,
    CppSource,
    CMakeFile,
  };

  struct GeneratedContent {
    FileType mType;
    std::string mName;
    std::string mText;
  };

  // This is read from CLI args
  struct GeneratorSettings {
    std::string mNamespace;
    std::string mCMakeTarget;
  };

  class Generator {
    GeneratorSettings mGeneratorSettings;
  public:
    Generator(const GeneratorSettings &generatorSettings) : mGeneratorSettings(generatorSettings) {}

    std::vector<GeneratedContent> Generate(const TranslatedProject &translatedProject);

  private:
    void GenerateCMakeLists(GeneratedContent &cmake, const TranslatedProject &translatedProject) const;
    void GenerateClassHeader(GeneratedContent &header, const Class &cls) const;
    void GenerateClassSource(GeneratedContent &source, const Class &cls) const;
    void GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
    void GenerateMethodDeclarations(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
    void GenerateMethodDefinitions(CodeBlock &codeBlock, const Class &cls) const;
    void GenerateForVisibility(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
  };
}
