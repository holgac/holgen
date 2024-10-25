#pragma once
#include "../TranslatedProject.h"
#include "../GeneratorSettings.h"

namespace holgen {
class Generator {
public:
  explicit Generator(const GeneratorSettings &generatorSettings,
                     const TranslatedProject &translatedProject) :
      mGeneratorSettings(generatorSettings), mTranslatedProject(translatedProject) {}

  virtual ~Generator() = default;
  virtual void Run(std::vector<GeneratedContent> &contents) const = 0;

protected:
  const GeneratorSettings &mGeneratorSettings;
  const TranslatedProject &mTranslatedProject;
  [[nodiscard]] std::string GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
                                                      bool isInHeader, bool isInsideClass) const;
  [[nodiscard]] std::string GenerateFunctionSignature(const Class &cls,
                                                      const ClassConstructor &ctor, bool isInHeader,
                                                      bool isInsideClass) const;
  [[nodiscard]] std::string GenerateFunctionSignature(const CFunction &func, bool isInHeader) const;
  [[nodiscard]] HeaderContainer PrepareIncludes(const Class &cls, bool isHeader) const;
  void AddCppComments(CodeBlock &codeBlock, const std::list<std::string> &comments) const;

  template <typename C>
  std::string StringifyTemplateParameters(const C &templateParameters) const {
    std::stringstream ss;
    ss << "template <";
    bool isFirst = true;
    for (const auto &templateParameter: templateParameters) {
      if (isFirst) {
        isFirst = false;
      } else {
        ss << ", ";
      }
      ss << templateParameter.mType << " " << templateParameter.mName;
    }
    ss << ">";
    return ss.str();
  }

  bool CanBeDefinedInHeader(const Class &cls, const MethodBase &method) const;

  bool CanBeDefinedInHeader(const Class &cls, const ClassMethod &method) const;

  std::string StringifyFieldDefinition(const ClassField &field) const;
  [[nodiscard]] CodeBlock GenerateDestructor(const Class &cls,
                                             Visibility visibility = Visibility::Public,
                                             bool isHeader = false) const;
};
} // namespace holgen
