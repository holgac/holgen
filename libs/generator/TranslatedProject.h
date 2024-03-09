#pragma once

#include "parser/DependencyGraph.h"
#include "TypeInfo.h"
#include "CodeBlock.h"
#include "HeaderContainer.h"

namespace holgen {

  enum class Visibility {
    Private,
    Protected,
    Public,
  };

  enum class Staticness {
    Static,
    NotStatic,
  };

  enum class Explicitness {
    Explicit,
    NotExplicit,
  };

  struct ClassField {
    Visibility mVisibility = Visibility::Private;
    Type mType;
    std::string mName;
    Staticness mStaticness = Staticness::NotStatic;
    std::string mDefaultValue;
    std::vector<std::string> mDefaultConstructorArguments;
    const FieldDefinition *mField = nullptr;
  };

  struct ClassMethodArgument {
    Type mType;
    std::string mName;
    std::string mDefaultValue;
  };

  struct TemplateParameter {
    std::string mType;
    std::string mName;
  };

  struct ClassMethodBase {
    Visibility mVisibility = Visibility::Public;
    CodeBlock mBody;
    std::vector<ClassMethodArgument> mArguments;
    std::vector<TemplateParameter> mTemplateParameters;
    bool mIsTemplateSpecialization = false;
  };

  struct Typedef {
    Type mSourceType;
    std::string mTargetType;
  };

  struct ClassMethod : ClassMethodBase {
    // TODO: ctor (name required, others optional)
    std::string mName;
    Type mReturnType;
    Constness mConstness = Constness::Const;
    Staticness mStaticness = Staticness::NotStatic;
  };

  struct ClassConstructorInitializer {
    std::string mDestination;
    std::string mValue;
  };

  struct ClassConstructor : ClassMethodBase {
    std::vector<ClassConstructorInitializer> mInitializerList;
    Explicitness mExplicitness = Explicitness::NotExplicit;
    // empty body and empty initializer list means = default.
    // bool isDeleted = false;
  };

  // Do a sample thing using lua, then figure out how to expose methods (both ways)
  // CRTP was useful for these when calling derived static methods from the base (when defining lua metaclass)
  // This is the unit that will be generated into multiple destinations (cpp header/src, maybe lua)
  struct Class {
    explicit Class(std::string name);

    const StructDefinition *mStruct = nullptr;
    const EnumDefinition *mEnum = nullptr;
    std::string mName;
    std::vector<ClassMethod> mMethods;
    std::vector<ClassConstructor> mConstructors;
    std::vector<ClassField> mFields;
    std::vector<TemplateParameter> mTemplateParameters;
    std::vector<Typedef> mTypedefs;
    HeaderContainer mHeaderIncludes;
    HeaderContainer mSourceIncludes;
    CodeBlock mGlobalForwardDeclarations;
    // CodeBlock mNamespaceForwardDeclarations;
    ClassField *GetField(const std::string &name);
    ClassMethod *GetMethod(const std::string &name, bool isConst);
  };

  struct TranslatedProject {
  public:
    explicit TranslatedProject(const ProjectDefinition &projectDefinition);
    const ProjectDefinition &mProject;
    const DependencyGraph mDependencyGraph;
    std::vector<Class> mClasses;
    [[nodiscard]] Class *GetClass(const std::string &name);
    [[nodiscard]] const Class *GetClass(const std::string &name) const;
  };

}
