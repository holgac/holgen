#pragma once

#include <vector>
#include <string>
#include <core/LineWithAction.h>
// Needed for Project (TODO: move to separate file and fwd declare)
#include "parser/Parser.h"
#include "CodeBlock.h"
#include "TypeInfo.h"


namespace holgen {

  enum class Visibility {
    Private,
    Protected,
    Public,
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
    ClassField* GetField(const std::string& name);
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
  public:
    TranslatedProject Translate(const ProjectDefinition &project) const;
  };
}
