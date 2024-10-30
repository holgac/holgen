#pragma once
#include "Common.h"
#include <string>
#include <optional>
#include <list>
#include <format>
#include "../TypeInfo.h"
#include "../CodeBlock.h"

namespace holgen {
class Class;
class CSharpMethod;

enum class CSharpVisibility {
  Private,
  Protected,
  Public,
  Internal,
};

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
  CSharpVisibility mVisibility = CSharpVisibility::Public;
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

  CSharpMethod(std::string name, CSharpType returnType,
               CSharpVisibility visibility = CSharpVisibility::Public,
               Staticness staticness = Staticness::NotStatic) :
      mName(std::move(name)), mReturnType(std::move(returnType)), mStaticness(staticness) {
    mVisibility = visibility;
  }
};

struct CSharpConstructor : CSharpMethodBase {
  CSharpConstructor(CSharpVisibility visibility = CSharpVisibility::Public) {
    mVisibility = visibility;
  }
};

struct CSharpClassField {
  CSharpClassField(std::string name, CSharpType type,
                   CSharpVisibility visibility = CSharpVisibility::Private,
                   Staticness staticness = Staticness::NotStatic,
                   std::optional<std::string> defaultValue = std::nullopt) :
      mType(std::move(type)), mName(std::move(name)), mStaticness(staticness),
      mVisibility(visibility), mDefaultValue(std::move(defaultValue)) {}

  CSharpType mType;
  std::string mName;
  Staticness mStaticness = Staticness::NotStatic;
  CSharpVisibility mVisibility = CSharpVisibility::Private;
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
  CSharpVisibility mVisibility = CSharpVisibility::Public;
  Staticness mStaticness = Staticness::NotStatic;
  CSharpClassType mType = CSharpClassType::Class;
  std::set<std::string> mUsingDirectives;
  std::list<CSharpMethod> mDelegates;
  std::list<CSharpMethod> mMethods;
  std::list<CSharpConstructor> mConstructors;
  std::list<CSharpClassField> mFields;
  std::list<CSharpClass> mInnerClasses;
  std::list<std::string> mAttributes;
  bool mIsAbstract = false;
};

} // namespace holgen

namespace std {
template <>
struct formatter<holgen::CSharpClassType> : formatter<std::string> {
  auto format(const holgen::CSharpClassType &type, format_context &ctx) const {
    switch (type) {
    case holgen::CSharpClassType::Class:
      return std::format_to(ctx.out(), "class");
    case holgen::CSharpClassType::Struct:
      return std::format_to(ctx.out(), "struct");
    case holgen::CSharpClassType::Interface:
      return std::format_to(ctx.out(), "interface");
    }
    THROW("Unexpected C# class type: {}", uint32_t(type));
  }
};

template <>
struct formatter<holgen::CSharpVisibility> : formatter<std::string> {
  auto format(const holgen::CSharpVisibility &visibility, format_context &ctx) const {
    switch (visibility) {
    case holgen::CSharpVisibility::Public:
      return std::format_to(ctx.out(), "public");
    case holgen::CSharpVisibility::Protected:
      return std::format_to(ctx.out(), "protected");
    case holgen::CSharpVisibility::Private:
      return std::format_to(ctx.out(), "private");
    case holgen::CSharpVisibility::Internal:
      return std::format_to(ctx.out(), "internal");
    }
    THROW("Unexpected visibility: {}", uint32_t(visibility));
  }
};
} // namespace std
