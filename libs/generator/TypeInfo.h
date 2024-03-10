#pragma once

#include <map>
#include <string>
#include <set>
#include <vector>

namespace holgen {
  struct TypeDefinition;

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

  // This name is too generic...
  struct Type {
    explicit Type(std::string name, PassByType passByType = PassByType::Value, Constness constness = Constness::NotConst)
    : mName(std::move(name)), mConstness(constness), mType(passByType) { }

    explicit Type(const TypeDefinition& typeDefinition, PassByType passByType = PassByType::Value, Constness constness = Constness::NotConst);


    std::string mName = "void";
    Constness mConstness;
    PassByType mType;
    Constexprness mConstexprness = Constexprness::NotConstexpr;
    std::vector<Type> mTemplateParameters;
    // This is for std::function which uses a different syntax
    // It's converted to string as [0]([1],[2], ...)
    std::vector<Type> mFunctionalTemplateParameters;
    std::string ToString() const;
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
    std::set<std::string> CppPrimitives;
    // Containers that allow random access
    std::set<std::string> CppIndexedContainers;
    // Map-like containers
    std::set<std::string> CppKeyedContainers;
    // All containers
    std::set<std::string> CppContainers;
    std::set<std::string> CppStableContainers;
    std::set<std::string> KeyableTypes;
    std::set<std::string> CppTypes;

  };

}
