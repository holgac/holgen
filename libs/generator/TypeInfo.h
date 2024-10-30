#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <memory>
#include "parser/ProjectDefinition.h"
#include "core/Exception.h"
#include "types/Common.h"

namespace holgen {
struct TranslatedProject;

template <typename RealType>
struct TypeBase {
  explicit TypeBase(std::string name) : mName(std::move(name)) {}

  TypeBase() = default;

  std::string mName = "void";
  std::vector<RealType> mTemplateParameters;

  bool operator==(const TypeBase &rhs) const {
    if (mName != rhs.mName || mTemplateParameters.size() != rhs.mTemplateParameters.size())
      return false;
    for (size_t i = 0; i < mTemplateParameters.size(); i++) {
      if (mTemplateParameters[i] != rhs.mTemplateParameters[i])
        return false;
    }
    return true;
  }
};

struct Type : TypeBase<Type> {
  explicit Type(std::string name, PassByType passByType = PassByType::Value,
                Constness constness = Constness::NotConst) :
      TypeBase(std::move(name)), mConstness(constness), mType(passByType) {}

  Type(const TranslatedProject &project, const DefinitionSource &definitionSource,
       const TypeDefinition &typeDefinition, PassByType passByType = PassByType::Value,
       Constness constness = Constness::NotConst);

  static Type ReturnType(const TranslatedProject &project, const FunctionDefinition &func);

  bool operator==(const Type &rhs) const;

  Constness mConstness;
  PassByType mType;
  Constexprness mConstexprness = Constexprness::NotConstexpr;
  // non-zero when the type is a pointer to pointer [to pointer, ...].
  uint32_t mPointerDepth = 0;
  // This is for std::function which uses a different syntax
  // It's converted to string as [0]([1],[2], ...)
  // TODO: Instead of a separate field, Type should natively support function types
  std::vector<Type> mFunctionalTemplateParameters;
  // Use this to specify a different type when generating the files.
  std::shared_ptr<Type> mFinalType;
  [[nodiscard]] std::string ToString(bool noTrailingSpace,
                                     bool ignoreConstForPrimitives = false) const;
  [[nodiscard]] std::string ToFullyQualifiedString(const TranslatedProject &project) const;
  void PreventCopying(bool addConst = true);
  [[nodiscard]] bool SupportsCopy(TranslatedProject &project) const;
  [[nodiscard]] bool SupportsMirroring(TranslatedProject &project) const;

private:
  [[nodiscard]] bool SupportsCopyOrMirroring(TranslatedProject &project,
                                             std::set<std::string> &seenClasses,
                                             bool forCopy) const;
};

enum class CSharpPassByType {
  Value,
  In,
  Ref,
  Out,
};

struct CSharpType : TypeBase<CSharpType> {
  CSharpType() = default;

  explicit CSharpType(const std::string &name) : TypeBase(name) {}

  [[nodiscard]] std::string ToString() const;
  // non-zero when the type is an array [of array, ...].
  uint32_t mArrayDepth = 0;
  CSharpPassByType mType = CSharpPassByType::Value;
};

enum class TypeUseCase {
  FieldDeclaration,
};

class TypeInfo {
  TypeInfo();

public:
  static TypeInfo &Get();
  std::map<std::string, std::string> TypeToCppType;
  std::set<std::string> IntegralTypes;
  std::set<std::string> SignedIntegralTypes;
  std::set<std::string> UnsignedIntegralTypes;
  std::set<std::string> FloatingPointTypes;
  std::set<std::string> CppPrimitives;
  std::set<std::string> CppBasicTypes;
  std::set<std::string> CppSmartPointers;
  // Containers that allow random access
  std::set<std::string> CppIndexedContainers;
  // std::list
  std::set<std::string> CppLists;
  // std::array
  std::set<std::string> CppFixedSizeContainers;
  // Map-like containers
  std::set<std::string> CppKeyedContainers;
  std::set<std::string> CppHashContainers;
  std::set<std::string> CppTreeContainers;
  // All containers
  std::set<std::string> CppContainers;
  std::set<std::string> CppStableContainers;
  std::set<std::string> CppSets;
  std::set<std::string> KeyableTypes;
  std::set<std::string> CppTypes;
  // Types that are valid but require no validation
  std::set<std::string> AllowlistedTypes;
  std::string GetUnsigned(const std::string &signedType);
};

} // namespace holgen

namespace std {
template <>
struct formatter<holgen::CSharpPassByType> : formatter<std::string> {
  auto format(const holgen::CSharpPassByType &visibility, format_context &ctx) const {
    switch (visibility) {
    case holgen::CSharpPassByType::Value:
      return std::format_to(ctx.out(), "");
    case holgen::CSharpPassByType::In:
      return std::format_to(ctx.out(), "in ");
    case holgen::CSharpPassByType::Out:
      return std::format_to(ctx.out(), "out ");
    case holgen::CSharpPassByType::Ref:
      return std::format_to(ctx.out(), "ref ");
    }
    THROW("Unexpected visibility: {}", uint32_t(visibility));
  }
};
} // namespace std
