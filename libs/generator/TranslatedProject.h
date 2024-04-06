#pragma once

#include <list>

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
    std::list<std::string> mDefaultConstructorArguments;
    const FieldDefinition *mField = nullptr;
    const EnumEntryDefinition *mEntry = nullptr;
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
    TemplateParameter(std::string type, std::string name);
    std::string mType;
    std::string mName;
  };

  struct ClassMethodBase {
    Visibility mVisibility = Visibility::Public;
    CodeBlock mBody;
    std::list<ClassMethodArgument> mArguments;
    std::list<TemplateParameter> mTemplateParameters;
    bool mIsTemplateSpecialization = false;
  protected:
    ClassMethodBase() = default;
  };

  struct Using {
    Using(
        Type sourceType,
        std::string targetType
    ) : mSourceType(std::move(sourceType)), mTargetType(std::move(targetType)) {}

    Type mSourceType;
    std::string mTargetType;
  };

  struct ClassMethod : ClassMethodBase {
    ClassMethod(
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
    const FunctionDefinition *mFunction = nullptr;
    bool mUserDefined = false;
    bool mExposeToLua = false;
    [[nodiscard]] const TemplateParameter *GetTemplateParameter(const std::string &name) const;
  };

  struct ClassConstructorInitializer {
    std::string mDestination;
    std::string mValue;

    ClassConstructorInitializer(std::string destination, std::string value) : mDestination(std::move(destination)),
                                                                              mValue(std::move(value)) {}
  };

  struct ClassConstructor : ClassMethodBase {
    ClassConstructor() = default;
    std::list<ClassConstructorInitializer> mInitializerList;
    Explicitness mExplicitness = Explicitness::NotExplicit;
    // empty body and empty initializer list means = default.
    // bool isDeleted = false;
  };

  struct ForwardDeclaration {
    std::string mType;
    std::string mName;

    bool operator<(const ForwardDeclaration &other) const {
      return mName < other.mName;
    }
  };

  // This is the unit that will be generated into multiple destinations (cpp header/src, maybe lua)
  struct Class {
    explicit Class(std::string name, const StructDefinition *_struct) : mStruct(_struct), mName(std::move(name)) {}

    explicit Class(std::string name, const EnumDefinition *_enum) : mEnum(_enum), mName(std::move(name)) {}

    explicit Class(std::string name) : mName(std::move(name)) {}

    const StructDefinition *mStruct = nullptr;
    const EnumDefinition *mEnum = nullptr;
    std::string mName;
    std::list<ClassMethod> mMethods;
    std::list<ClassConstructor> mConstructors;
    std::list<ClassField> mFields;
    std::list<TemplateParameter> mTemplateParameters;
    std::list<Using> mUsings;
    HeaderContainer mHeaderIncludes;
    HeaderContainer mSourceIncludes;
    std::set<ForwardDeclaration> mGlobalForwardDeclarations;
    [[nodiscard]] ClassField *GetField(const std::string &name);
    [[nodiscard]] const ClassField *GetField(const std::string &name) const;
    [[nodiscard]] ClassMethod *GetMethod(const std::string &name, Constness constness);
    [[nodiscard]] const Using *GetUsing(const std::string &name) const;
    [[nodiscard]] const ForwardDeclaration *GetForwardDeclaration(const std::string &name) const;
    [[nodiscard]] const TemplateParameter *GetTemplateParameter(const std::string &name) const;

    [[nodiscard]] auto GetMethods(const std::string &name) const {
      return NameFilterForEachWrapper(name, mMethods);
    }
  };

  struct TranslatedProject {
  public:
    explicit TranslatedProject(const ProjectDefinition &projectDefinition);
    const ProjectDefinition &mProject;
    const DependencyGraph mDependencyGraph;
    std::list<Class> mClasses;
    [[nodiscard]] Class *GetClass(const std::string &name);
    [[nodiscard]] const Class *GetClass(const std::string &name) const;
  };

}
