#pragma once

#include <list>
#include <utility>

#include "CodeBlock.h"
#include "HeaderContainer.h"
#include "TypeInfo.h"
#include "parser/DependencyGraph.h"

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

enum class Noexceptness {
  Noexcept,
  NotNoexcept,
};

enum class Explicitness {
  Explicit,
  NotExplicit,
};

enum class DefaultDelete {
  Default,
  Delete,
  Neither,
};

struct ClassField {
  ClassField(std::string name, Type type, Visibility visibility = Visibility::Private,
             Staticness staticness = Staticness::NotStatic,
             std::optional<std::string> defaultValue = std::nullopt) :
      mType(std::move(type)), mName(std::move(name)), mStaticness(staticness),
      mVisibility(visibility), mDefaultValue(std::move(defaultValue)) {}

  Type mType;
  std::string mName;
  Staticness mStaticness = Staticness::NotStatic;
  Visibility mVisibility = Visibility::Private;
  std::optional<std::string> mDefaultValue = std::nullopt;
  std::list<std::string> mDefaultConstructorArguments;
  std::list<std::string> mComments;
  const FieldDefinition *mField = nullptr;
};

struct ClassMethodArgument {
  ClassMethodArgument(std::string name, Type type,
                      std::optional<std::string> defaultValue = std::nullopt) :
      mType(std::move(type)), mName(std::move(name)), mDefaultValue(std::move(defaultValue)) {}

  Type mType;
  std::string mName;
  std::optional<std::string> mDefaultValue;
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
  std::list<std::string> mComments;
  bool mIsTemplateSpecialization = false;
  bool mUserDefined = false;
  DefaultDelete mDefaultDelete = DefaultDelete::Neither;
  const FunctionDefinition *mFunction = nullptr;
  Noexceptness mNoexceptness = Noexceptness::NotNoexcept;

protected:
  ClassMethodBase() = default;
};

struct Using {
  Using(Type sourceType, std::string targetType) :
      mSourceType(std::move(sourceType)), mTargetType(std::move(targetType)) {}

  Type mSourceType;
  std::string mTargetType;
};

struct ClassMethod : ClassMethodBase {
  ClassMethod(std::string name, Type returnType, Visibility visibility = Visibility::Public,
              Constness constness = Constness::Const,
              Staticness staticness = Staticness::NotStatic) :
      mName(std::move(name)), mReturnType(std::move(returnType)), mConstness(constness),
      mStaticness(staticness) {
    mVisibility = visibility;
  }

  std::string mName;
  Type mReturnType;
  // TODO: bit flags?
  Constness mConstness;
  Staticness mStaticness;
  Constexprness mConstexprness = Constexprness::NotConstexpr;
  bool mExposeToLua = false;
  [[nodiscard]] const TemplateParameter *GetTemplateParameter(const std::string &name) const;
};

struct ClassConstructorInitializer {
  std::string mDestination;
  std::string mValue;

  ClassConstructorInitializer(std::string destination, std::string value) :
      mDestination(std::move(destination)), mValue(std::move(value)) {}
};

struct ClassConstructor : ClassMethodBase {
  ClassConstructor() = default;
  std::list<ClassConstructorInitializer> mInitializerList;
  Explicitness mExplicitness = Explicitness::NotExplicit;
};

struct ClassDestructor : ClassMethodBase {
  ClassDestructor() = default;
  bool IsEmpty() const;
};

enum class ClassType {
  Class,
  Struct,
};

struct ClassEnumEntry {
  std::string mName;
  std::string mValue;
  std::list<std::string> mComments;
  const EnumEntryDefinition *mEntry;

  ClassEnumEntry(std::string name, std::string value, const EnumEntryDefinition *entry = nullptr) :
      mName(std::move(name)), mValue(std::move(value)), mEntry(entry) {}
};

struct ClassEnum {
  std::string mName;
  Visibility mVisibility = Visibility::Public;
  std::string mUnderlyingType;
  std::list<ClassEnumEntry> mEntries;
  std::list<std::string> mComments;

  ClassEnum(std::string name, std::string underlyingType,
            Visibility visibility = Visibility::Public) :
      mName(std::move(name)), mVisibility(visibility), mUnderlyingType(std::move(underlyingType)) {}

  [[nodiscard]] const ClassEnumEntry *GetEntry(const std::string &name) const;
};

// This is the unit that will be generated into multiple destinations (cpp header/src, maybe lua)
struct Class {
  explicit Class(std::string name, std::string _namespace, const StructDefinition *_struct) :
      mStruct(_struct), mName(std::move(name)), mNamespace(std::move(_namespace)) {}

  explicit Class(std::string name, std::string _namespace, const EnumDefinition *_enum) :
      mEnum(_enum), mName(std::move(name)), mNamespace(std::move(_namespace)) {}

  explicit Class(std::string name, std::string _namespace) :
      mName(std::move(name)), mNamespace(std::move(_namespace)) {}

  const StructDefinition *mStruct = nullptr;
  const EnumDefinition *mEnum = nullptr;
  std::string mName;
  std::list<ClassMethod> mMethods;
  std::list<ClassConstructor> mConstructors;
  ClassDestructor mDestructor;
  std::list<ClassField> mFields;
  std::list<TemplateParameter> mTemplateParameters;
  std::list<std::string> mTemplateSpecializations;
  std::list<Using> mUsings;
  std::list<Class> mSpecializations;
  std::list<ClassEnum> mNestedEnums;
  HeaderContainer mHeaderIncludes;
  HeaderContainer mSourceIncludes;
  ClassType mClassType = ClassType::Class;
  std::string mNamespace;
  std::list<std::string> mBaseClasses;
  std::list<std::string> mComments;
  [[nodiscard]] ClassField *GetField(const std::string &name);
  [[nodiscard]] ClassField *GetFieldFromDefinitionName(const std::string &name);
  [[nodiscard]] const ClassField *GetField(const std::string &name) const;
  [[nodiscard]] const ClassField *GetIdField() const;
  [[nodiscard]] ClassMethod *GetMethod(const std::string &name, Constness constness);
  [[nodiscard]] const Using *GetUsing(const std::string &name) const;
  [[nodiscard]] const TemplateParameter *GetTemplateParameter(const std::string &name) const;
  [[nodiscard]] const ClassEnum *GetNestedEnum(const std::string &name) const;
  [[nodiscard]] ClassConstructor *GetMoveConstructor();
  [[nodiscard]] ClassConstructor *GetCopyConstructor();
  [[nodiscard]] ClassConstructor *GetDefaultConstructor();
  [[nodiscard]] ClassMethod *GetMoveAssignment();
  [[nodiscard]] ClassMethod *GetCopyAssignment();

  [[nodiscard]] auto GetMethods(const std::string &name) const {
    return NameFilterForEachWrapper(name, mMethods);
  }

  using VariantData = std::map<std::string, std::vector<ClassField *>>;
  // returns a {variant type field name -> [variant field name]} dict
  [[nodiscard]] VariantData GetVariantData();
};

struct TranslatedProject {
public:
  explicit TranslatedProject(const ProjectDefinition &projectDefinition);
  const ProjectDefinition &mProject;
  const DependencyGraph mDependencyGraph;
  std::list<Class> mClasses;
  [[nodiscard]] Class *GetClass(const std::string &name);
  [[nodiscard]] const Class *GetClass(const std::string &name) const;
  [[nodiscard]] std::vector<std::pair<Class *, const EnumEntryDefinition *>>
      GetVariantClassesOfEnum(const std::string &enumName);
};

} // namespace holgen
