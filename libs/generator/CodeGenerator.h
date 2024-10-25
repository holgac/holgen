#pragma once

#include <string>
#include "FileType.h"
#include "Translator.h"

namespace holgen {
struct GeneratedContent {
  FileType mType;
  std::string mName;
  CodeBlock mBody;
};

namespace GeneratorFeatureFlag {
enum Entry : uint64_t {
  SwigLua = 1,
  SwigCSharp = 2,
  SwigMask = SwigLua | SwigCSharp,
};
}; // namespace GeneratorFeatureFlag

// This is read from CLI args
struct GeneratorSettings {
  std::string mProjectName;
  std::string mConfigHeader;
  GeneratorFeatureFlag::Entry mFeatureFlags = GeneratorFeatureFlag::Entry(0);

  [[nodiscard]] bool IsFeatureEnabled(const GeneratorFeatureFlag::Entry featureFlag) const {
    return (mFeatureFlags & featureFlag) != 0;
  }

  void EnableFeature(const GeneratorFeatureFlag::Entry featureFlag) {
    mFeatureFlags = GeneratorFeatureFlag::Entry(mFeatureFlags | featureFlag);
  }
};

class CodeGenerator {
  GeneratorSettings mGeneratorSettings;
  const TranslatedProject *mTranslatedProject = nullptr;

public:
  CodeGenerator(const GeneratorSettings &generatorSettings) :
      mGeneratorSettings(generatorSettings) {}

  std::vector<GeneratedContent> Generate(const TranslatedProject &translatedProject);

private:
  void GenerateCMakeLists(GeneratedContent &cmake,
                          const TranslatedProject &translatedProject) const;
  void GenerateHolgenHeader(GeneratedContent &header) const;
  void GenerateClassHeader(GeneratedContent &header, const Class &cls,
                           const HeaderContainer &headers) const;
  void GenerateClassSource(GeneratedContent &source, const Class &cls,
                           const HeaderContainer &headers) const;
  void GenerateClassModifiableSource(GeneratedContent &source, const Class &cls) const;
  void GenerateClassDeclarationsForHeader(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateUsingsForHeader(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateCFunctionsForHeader(CodeBlock &codeBlock, const Class &cls) const;
  HeaderContainer PrepareIncludes(const Class &cls, bool isHeader) const;
  void GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls,
                                 Visibility visibility) const;
  void GenerateConstructorsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility,
                                     bool isInsideClass) const;
  [[nodiscard]] CodeBlock GenerateDestructor(const Class &cls,
                                             Visibility visibility = Visibility::Public,
                                             bool isHeader = false) const;
  void GenerateNestedEnumsForHeader(CodeBlock &codeBlock, const Class &cls,
                                    Visibility visibility) const;
  void GenerateMethodsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility,
                                bool isInsideClass) const;
  [[nodiscard]] CodeBlock GenerateMethodsForSource(const Class &cls) const;
  [[nodiscard]] CodeBlock GenerateFieldsForSource(const Class &cls) const;
  [[nodiscard]] CodeBlock GenerateConstructorsForSource(const Class &cls) const;
  void GenerateCFunctionsForSource(CodeBlock &codeBlock, const Class &cls) const;
  void GenerateForVisibility(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const;
  std::string GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
                                        bool isInHeader, bool isInsideClass) const;
  std::string GenerateFunctionSignature(const Class &cls, const ClassConstructor &ctor,
                                        bool isInHeader, bool isInsideClass) const;
  std::string GenerateFunctionSignature(const CFunction &func, bool isInHeader) const;
  std::string GenerateClassDeclaration(const Class &cls) const;
  void GenerateClassDefinition(const Class &cls, CodeBlock &codeBlock) const;
  void GenerateSwigInterface(GeneratedContent &swig,
                             const TranslatedProject &translatedProject) const;
  void GenerateSwigInterfaceIncludes(const Class &cls, CodeBlock &codeBlock) const;
  void GenerateSwigInterfaceForStruct(const Class &cls, CodeBlock &codeBlock) const;
  void GenerateSwigInterfaceForEnum(const Class &cls, CodeBlock &codeBlock) const;
};
} // namespace holgen
