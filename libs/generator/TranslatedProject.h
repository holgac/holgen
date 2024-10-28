#pragma once

#include <list>
#include <utility>

#include "CodeBlock.h"
#include "HeaderContainer.h"
#include "TypeInfo.h"
#include "parser/DependencyGraph.h"
#include "core/Exception.h"

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

enum class Virtuality {
  NotVirtual,
  Virtual,
  PureVirtual,
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

struct MethodBase {
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
  MethodBase() = default;
};

struct Using {
  Using(Type sourceType, std::string targetType) :
      mSourceType(std::move(sourceType)), mTargetType(std::move(targetType)) {}

  Type mSourceType;
  std::string mTargetType;
};

struct CFunction : MethodBase {
  CFunction(std::string name, Type returnType, const ClassMethod *method) :
      mName(std::move(name)), mReturnType(std::move(returnType)), mMethod(method) {}

  std::string mName;
  Type mReturnType;
  const ClassMethod *mMethod;
};

struct ClassMethod : MethodBase {
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
  Virtuality mVirtuality = Virtuality::NotVirtual;
  bool mExposeToScript = false;
  bool mFunctionPointer = false;
  [[nodiscard]] const TemplateParameter *GetTemplateParameter(const std::string &name) const;
  [[nodiscard]] bool IsStatic(const Class& cls) const;
};

struct ClassConstructorInitializer {
  std::string mDestination;
  std::string mValue;

  ClassConstructorInitializer(std::string destination, std::string value) :
      mDestination(std::move(destination)), mValue(std::move(value)) {}
};

struct ClassConstructor : MethodBase {
  ClassConstructor() = default;
  std::list<ClassConstructorInitializer> mInitializerList;
  Explicitness mExplicitness = Explicitness::NotExplicit;
};

struct ClassDestructor : MethodBase {
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

struct BaseClass {
  BaseClass(Visibility visibility, Type type) : mType(std::move(type)), mVisibility(visibility) {}

  Type mType;
  Visibility mVisibility = Visibility::Public;
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
  // functions defined in extern C in the class header, typically used to wrap methods
  std::list<CFunction> mCFunctions;
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
  CodeBlock mHeaderCustomSection;
  ClassType mClassType = ClassType::Class;
  std::string mNamespace;
  std::list<BaseClass> mBaseClasses;
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
  [[nodiscard]] bool HasUserDefinedMethods() const;

  [[nodiscard]] auto GetMethods(const std::string &name) const {
    return NameFilterForEachWrapper(name, mMethods);
  }

  using VariantData = std::map<std::string, std::vector<ClassField *>>;
  // returns a {variant type field name -> [variant field name]} dict
  [[nodiscard]] VariantData GetVariantData();
  [[nodiscard]] bool HasVirtualMethods() const;
  [[nodiscard]] bool IsAbstract() const;
  [[nodiscard]] bool IsProxyable() const;
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

namespace std {
template <>
struct formatter<holgen::Visibility> : formatter<std::string> {
  auto format(const holgen::Visibility &visibility, format_context &ctx) const {
    switch (visibility) {
    case holgen::Visibility::Public:
      return std::format_to(ctx.out(), "public");
    case holgen::Visibility::Protected:
      return std::format_to(ctx.out(), "protected");
    case holgen::Visibility::Private:
      return std::format_to(ctx.out(), "private");
    }
    THROW("Unexpected visibility: {}", uint32_t(visibility));
  }
};
} // namespace std
