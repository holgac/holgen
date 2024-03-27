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
    ClassField(
        std::string name,
        Type type,
        Visibility visibility = Visibility::Private,
        Staticness staticness = Staticness::NotStatic,
        std::string defaultValue = ""
    ) : mType(std::move(type)), mName(std::move(name)), mStaticness(staticness), mVisibility(visibility),
        mDefaultValue(std::move(defaultValue)) {
    }

    Type mType;
    std::string mName;
    Staticness mStaticness = Staticness::NotStatic;
    Visibility mVisibility = Visibility::Private;
    std::string mDefaultValue;
    std::vector<std::string> mDefaultConstructorArguments;
    const FieldDefinition *mField = nullptr;
  };

  struct ClassMethodArgument {
    ClassMethodArgument(
        std::string name,
        Type type,
        std::string defaultValue = ""
    ) : mType(std::move(type)), mName(std::move(name)), mDefaultValue(std::move(defaultValue)) {
    }

    Type mType;
    std::string mName;
    std::string mDefaultValue;
  };

  struct TemplateParameter {
    // TODO: ctor
    // TODO: type here?
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

  struct Using {
    Using(
        Type sourceType,
        std::string targetType
    ) : mSourceType(sourceType), mTargetType(targetType) {}

    Type mSourceType;
    std::string mTargetType;
  };

  struct ClassMethod : ClassMethodBase {
    explicit ClassMethod(
        std::string name, Type returnType,
        Visibility visibility = Visibility::Public,
        Constness constness = Constness::Const,
        Staticness staticness = Staticness::NotStatic
    ) : mName(std::move(name)), mReturnType(std::move(returnType)), mConstness(constness),
        mStaticness(staticness) { mVisibility = visibility; }

    std::string mName;
    Type mReturnType;
    // TODO: bit flags?
    Constness mConstness;
    Staticness mStaticness;
    Constexprness mConstexprness = Constexprness::NotConstexpr;
    bool mUserDefined = false;
    bool mExposeToLua = false;
  };

  struct ClassConstructorInitializer {
    std::string mDestination;
    std::string mValue;

    ClassConstructorInitializer(std::string destination, std::string value) : mDestination(std::move(destination)),
                                                                              mValue(std::move(value)) {}
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
    explicit Class(std::string name, const StructDefinition *_struct) : mStruct(_struct), mName(std::move(name)) {}

    explicit Class(std::string name, const EnumDefinition *_enum) : mEnum(_enum), mName(std::move(name)) {}

    explicit Class(std::string name) : mName(std::move(name)) {}

    const StructDefinition *mStruct = nullptr;
    const EnumDefinition *mEnum = nullptr;
    std::string mName;
    std::vector<ClassMethod> mMethods;
    std::vector<ClassConstructor> mConstructors;
    std::vector<ClassField> mFields;
    std::vector<TemplateParameter> mTemplateParameters;
    std::vector<Using> mUsings;
    HeaderContainer mHeaderIncludes;
    HeaderContainer mSourceIncludes;
    std::set<std::string> mGlobalForwardDeclarations;
    ClassField *GetField(const std::string &name);
    ClassMethod *GetMethod(const std::string &name, Constness constness);
    const Using *GetUsing(const std::string &name) const;
  };

  struct TranslatedProject {
  public:
    explicit TranslatedProject(const ProjectDefinition &projectDefinition);
    const ProjectDefinition &mProject;
    const DependencyGraph mDependencyGraph;
    std::vector<Class> mClasses;
    // TODO: AddClass that checks name colls
    [[nodiscard]] Class *GetClass(const std::string &name);
    [[nodiscard]] const Class *GetClass(const std::string &name) const;
  };

}
