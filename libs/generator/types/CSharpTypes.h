#pragma once
#include "Common.h"
#include <string>
#include <optional>
#include <list>
#include "../TypeInfo.h"
#include "../CodeBlock.h"

namespace holgen {
class Class;
class CSharpMethod;

enum class CSharpClassType {
  Class,
  Struct,
  Interface
};

struct CSharpMethodArgument {
  CSharpMethodArgument(std::string name, CSharpType type,
                       std::optional<std::string> defaultValue = std::nullopt) :
      mType(std::move(type)), mName(std::move(name)), mDefaultValue(std::move(defaultValue)) {}

  CSharpType mType;
  std::string mName;
  std::optional<std::string> mDefaultValue;
  std::list<std::string> mAttributes;
};

struct CSharpMethodBase {
  Visibility mVisibility = Visibility::Public;
  std::list<CSharpMethodArgument> mArguments;
  std::list<std::string> mComments;
  CodeBlock mBody;
  Virtuality mVirtuality = Virtuality::NotVirtual;
  std::list<std::string> mAttributes;
};

struct CSharpMethod : CSharpMethodBase {
  std::string mName;
  CSharpType mReturnType;
  Staticness mStaticness = Staticness::NotStatic;

  CSharpMethod(std::string name, CSharpType returnType, Visibility visibility = Visibility::Public,
               Staticness staticness = Staticness::NotStatic) :
      mName(std::move(name)), mReturnType(std::move(returnType)), mStaticness(staticness) {
    mVisibility = visibility;
  }
};

struct CSharpConstructor : CSharpMethodBase {
  CSharpConstructor(Visibility visibility = Visibility::Public) {
    mVisibility = visibility;
  }
};

struct CSharpClassField {
  CSharpClassField(std::string name, CSharpType type, Visibility visibility = Visibility::Private,
                   Staticness staticness = Staticness::NotStatic,
                   std::optional<std::string> defaultValue = std::nullopt) :
      mType(std::move(type)), mName(std::move(name)), mStaticness(staticness),
      mVisibility(visibility), mDefaultValue(std::move(defaultValue)) {}

  CSharpType mType;
  std::string mName;
  Staticness mStaticness = Staticness::NotStatic;
  Visibility mVisibility = Visibility::Private;
  std::optional<std::string> mDefaultValue = std::nullopt;
  std::list<std::string> mComments;
  std::optional<CSharpMethodBase> mGetter;
  std::optional<CSharpMethodBase> mSetter;
};

struct CSharpClass {
  explicit CSharpClass(std::string name, Class *cls) : mClass(cls), mName(std::move(name)) {}

  explicit CSharpClass(std::string name) : mName(std::move(name)) {}

  Class *mClass = nullptr;
  std::string mName;
  Visibility mVisibility = Visibility::Public;
  Staticness mStaticness = Staticness::NotStatic;
  CSharpClassType mType = CSharpClassType::Class;
  std::set<std::string> mUsingDirectives;
  std::list<CSharpMethod> mDelegates;
  std::list<CSharpMethod> mMethods;
  std::list<CSharpConstructor> mConstructors;
  std::list<CSharpClassField> mFields;
  std::list<CSharpClass> mInnerClasses;
  std::list<std::string> mAttributes;
};

} // namespace holgen
