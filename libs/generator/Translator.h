#pragma once

#include <vector>
#include <string>
#include <core/LineWithAction.h>
// Needed for Project (TODO: move to separate file and fwd declare)
#include "parser/Parser.h"


namespace holgen {

  class GeneratorException : std::exception {
    const char *mMsg;
  public:
    explicit GeneratorException(const char *msg) : mMsg(msg) {

    }

    const char *what() { return mMsg; }
  };


  enum class Visibility {
    Private,
    Protected,
    Public,
  };

  enum class CodeUnitType {
    Code,
    Indentation,
  };

  struct CodeBlock {
    std::vector<CodeUnitType> mContents;
    std::vector<std::string> mLines;
    std::vector<ssize_t> mIndentations;

    void Indent(ssize_t amount) {
      mContents.push_back(CodeUnitType::Indentation);
      mIndentations.push_back(amount);
    }

    LineWithAction Line() {
      return {[this](const auto &s) -> void { this->AddLine(s); }};
    }

    void Add(const CodeBlock& codeBlock);


    void AddLine(const std::string &line) {
      mContents.push_back(CodeUnitType::Code);
      mLines.push_back(line);
    }
  };

  enum class PassByType {
    Value,
    Reference,
    Pointer,
  };

  // This name is too generic...
  struct Type {
    std::string mName = "void";
    bool mIsConst = false;
    PassByType mType = PassByType::Value;
    std::vector<Type> mTemplateParameters;

    // structs are used only as data storage here - having a function defeats this
    // Can put this in anon namespace if only Translator uses it
    bool IsPrimitive() const;
  };

  struct ClassField {
    Visibility mVisibility = Visibility::Private;
    Type mType;
    std::string mName;
  };

  struct ClassMethodArgument {
    Type mType;
    std::string mName;
  };

  struct ClassMethodBase {
    Visibility mVisibility = Visibility::Public;
    CodeBlock mBody;
    std::vector<ClassMethodArgument> mArguments;
  };

  struct TemplateParameter {
    std::string mType;
    std::string mName;
  };
  struct ClassMethod : ClassMethodBase {
    std::string mName;
    Type mType;
    bool mIsConst = true;
    bool mIsStatic = false;
    std::vector<TemplateParameter> mTemplateParameters;
    bool mIsTemplateSpecialization = false;
  };

  struct ClassConstructorInitializer {
    std::string mDestination;
    std::string mValue;
  };

  struct ClassConstructor : ClassMethodBase {
    std::vector<ClassConstructorInitializer> mInitializerList;
    // empty body and empty initializer list means = default.
    // bool isDeleted = false;
  };

  // Do a sample thing using lua, then figure out how to expose methods (both ways)
  // CRTP was useful for these when calling derived static methods from the base (when defining lua metaclass)
  // This is the unit that will be generated into multiple destinations (cpp header/src, maybe lua)
  struct Class {
    std::string mName;
    std::vector<ClassMethod> mMethods;
    std::vector<ClassConstructor> mConstructors;
    std::vector<ClassField> mFields;
  };

  // This is the unit that will actually be generated into multiple files
  // cpp header/source, lua, cmake
  struct TranslatedProject {
    std::vector<Class> mClasses;
  };

  // This is more like a translator
  class Translator {
    void GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const;
    void ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const;
    void ProcessType(Type &type, const TypeDefinition &typeDefinition) const;
    void GenerateParseJson(Class &generatedClass, const StructDefinition &structDefinition) const;
    void GenerateJsonHelper(Class &generatedClass) const;
  public:
    TranslatedProject Translate(const ProjectDefinition &project) const;
  };
}
