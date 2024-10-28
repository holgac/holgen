#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include "parser/ProjectDefinition.h"

namespace holgen {
struct TranslatedProject;
enum class PassByType {
  Value,
  Reference,
  Pointer,
  MoveReference,
};

enum class Constexprness {
  Constexpr,
  NotConstexpr,
};

struct Type {
  explicit Type(std::string name, PassByType passByType = PassByType::Value,
                Constness constness = Constness::NotConst) :
      mName(std::move(name)), mConstness(constness), mType(passByType) {}

  Type(const TranslatedProject &project, const DefinitionSource &definitionSource,
       const TypeDefinition &typeDefinition, PassByType passByType = PassByType::Value,
       Constness constness = Constness::NotConst);

  static Type ReturnType(const TranslatedProject& project, const FunctionDefinition& func);

  bool operator==(const Type &rhs) const {
    if (mName != rhs.mName || mConstness != rhs.mConstness || mType != rhs.mType ||
        mConstexprness != rhs.mConstexprness ||
        mTemplateParameters.size() != rhs.mTemplateParameters.size() ||
        mFunctionalTemplateParameters.size() != rhs.mFunctionalTemplateParameters.size())
      return false;
    for (size_t i = 0; i < mTemplateParameters.size(); i++) {
      if (mTemplateParameters[i] != rhs.mTemplateParameters[i])
        return false;
    }
    for (size_t i = 0; i < mFunctionalTemplateParameters.size(); i++) {
      if (mFunctionalTemplateParameters[i] != rhs.mFunctionalTemplateParameters[i])
        return false;
    }
    return true;
  }

  std::string mName = "void";
  Constness mConstness;
  PassByType mType;
  Constexprness mConstexprness = Constexprness::NotConstexpr;
  std::vector<Type> mTemplateParameters;
  // non-zero when the type is a pointer to pointer [to pointer, ...].
  uint32_t mPointerDepth = 0;
  // This is for std::function which uses a different syntax
  // It's converted to string as [0]([1],[2], ...)
  // TODO: Instead of a separate field, Type should natively support function types
  std::vector<Type> mFunctionalTemplateParameters;
  [[nodiscard]] std::string ToString(bool noTrailingSpace, bool ignoreConstForPrimitives = false) const;
  [[nodiscard]] std::string ToFullyQualifiedString(const TranslatedProject& project) const;
  void PreventCopying(bool addConst = true);
  [[nodiscard]] bool SupportsCopy(TranslatedProject &project) const;
  [[nodiscard]] bool SupportsMirroring(TranslatedProject &project) const;
private:
  [[nodiscard]] bool SupportsCopyOrMirroring(TranslatedProject &project,
                                std::set<std::string> &seenClasses, bool forCopy) const;
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
