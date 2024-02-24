#pragma once

#include "parser/DependencyGraph.h"
#include "TypeInfo.h"
#include "CodeBlock.h"

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
    ClassField *GetField(const std::string &name);
  };

  struct TranslatedProject {
  public:
    explicit TranslatedProject(const ProjectDefinition &projectDefinition);
    const ProjectDefinition &mProject;
    const DependencyGraph mDependencyGraph;
    std::vector<Class> mClasses;
  };

}