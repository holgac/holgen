#pragma once

#include <map>
#include <string>
#include <set>
#include <vector>

namespace holgen {
  struct TypeDefinition;
  struct TranslatedProject;

  enum class PassByType {
    Value,
    Reference,
    Pointer,
    MoveReference,
  };

  enum class Constness {
    Const,
    NotConst,
  };

  enum class Constexprness {
    Constexpr,
    NotConstexpr,
  };

  struct Type {
    explicit Type(std::string name, PassByType passByType = PassByType::Value,
                  Constness constness = Constness::NotConst)
        : mName(std::move(name)), mConstness(constness), mType(passByType) {}

    Type(const TranslatedProject &project, const TypeDefinition &typeDefinition,
                  PassByType passByType = PassByType::Value, Constness constness = Constness::NotConst);

    bool operator==(const Type &rhs) const {
      if (mName != rhs.mName || mConstness != rhs.mConstness || mType != rhs.mType
          || mConstexprness != rhs.mConstexprness || mTemplateParameters.size() != rhs.mTemplateParameters.size()
          || mFunctionalTemplateParameters.size() != rhs.mFunctionalTemplateParameters.size())
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
    // This is for std::function which uses a different syntax
    // It's converted to string as [0]([1],[2], ...)
    // TODO: Instead of a separate field, Type should natively support function types
    std::vector<Type> mFunctionalTemplateParameters;
    [[nodiscard]] std::string ToString(bool noTrailingSpace) const;
    void PreventCopying(bool addConst = true);
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
    // Containers that allow random access
    std::set<std::string> CppIndexedContainers;
    // std::array
    std::set<std::string> CppFixedSizeContainers;
    // Map-like containers
    std::set<std::string> CppKeyedContainers;
    // All containers
    std::set<std::string> CppContainers;
    std::set<std::string> CppStableContainers;
    std::set<std::string> CppSets;
    std::set<std::string> KeyableTypes;
    std::set<std::string> CppTypes;
    // Types that are valid but require no validation
    std::set<std::string> AllowlistedTypes;

  };

}
