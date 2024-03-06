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

  // This name is too generic...
  struct Type {
    std::string mName = "void";
    Constness mConstness = Constness::NotConst;
    PassByType mType = PassByType::Value;
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
    void ConvertToType(
        Type &type,
        const TypeDefinition &typeDefinition
    );
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
