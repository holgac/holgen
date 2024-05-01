#pragma once

#include "Translator.h"
#include "FileType.h"
#include <string>


namespace holgen {
  struct GeneratedContent {
    FileType mType;
    std::string mName;
    CodeBlock mBody;
  };

  // This is read from CLI args
  struct GeneratorSettings {
    std::string mCMakeTarget;
    std::string mConfigHeader;
  };

  class CodeGenerator {
    GeneratorSettings mGeneratorSettings;
    const TranslatedProject *mTranslatedProject = nullptr;
  public:
    CodeGenerator(const GeneratorSettings &generatorSettings) : mGeneratorSettings(generatorSettings) {}

    std::vector<GeneratedContent> Generate(const TranslatedProject &translatedProject);

  private:
    void GenerateCMakeLists(GeneratedContent &cmake, const TranslatedProject &translatedProject) const;
    void GenerateHolgenHeader(GeneratedContent &header) const;
    void GenerateClassHeader(GeneratedContent &header, const Class &cls) const;
    void GenerateClassSource(GeneratedContent &source, const Class &cls) const;
    void GenerateClassModifiableSource(GeneratedContent &source, const Class &cls) const;
    void GenerateClassDeclarationsForHeader(CodeBlock &codeBlock, const Class &cls) const;
    void GenerateUsingsForHeader(CodeBlock &codeBlock, const Class &cls) const;
    void GenerateIncludes(CodeBlock &codeBlock, const Class &cls, bool isHeader) const;
    void GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
    void GenerateConstructorsForHeader(
        CodeBlock &codeBlock, const Class &cls, Visibility visibility, bool isInsideClass) const;
    [[nodiscard]] CodeBlock GenerateDestructor(
        const Class &cls, Visibility visibility = Visibility::Public, bool isHeader = false) const;
    void GenerateNestedEnumsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
    void GenerateMethodsForHeader(
        CodeBlock &codeBlock, const Class &cls, Visibility visibility, bool isInsideClass) const;
    [[nodiscard]] CodeBlock GenerateMethodsForSource(const Class &cls) const;
    [[nodiscard]] CodeBlock GenerateFieldsForSource(const Class &cls) const;
    [[nodiscard]] CodeBlock GenerateConstructorsForSource(const Class &cls) const;
    void GenerateForVisibility(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
    std::string GenerateFunctionSignature(
        const Class &cls, const ClassMethod &method, bool isInHeader, bool isInsideClass) const;
    std::string GenerateClassDeclaration(const Class &cls) const;
    void GenerateClassDefinition(const Class &cls, CodeBlock &codeBlock) const;
  };
}
