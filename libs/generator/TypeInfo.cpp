#include "TypeInfo.h"
#include <sstream>
#include "parser/Parser.h"
#include "TranslatedProject.h"

namespace holgen {
  TypeInfo::TypeInfo() {
    TypeToCppType = {
        {"s8",            "int8_t"},
        {"s16",           "int16_t"},
        {"s32",           "int32_t"},
        {"s64",           "int64_t"},
        {"u8",            "uint8_t"},
        {"u16",           "uint16_t"},
        {"u32",           "uint32_t"},
        {"u64",           "uint64_t"},
        {"float",         "float"},
        {"double",        "double"},
        {"bool",          "bool"},
        {"string",        "std::string"},
        {"vector",        "std::vector"},
        {"deque",         "std::deque"},
        {"map",           "std::map"},
        {"unordered_map", "std::unordered_map"},
        {"set",           "std::set"},
        {"unordered_set", "std::unordered_set"},
    };

    SignedIntegralTypes = {
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "ssize_t",
    };
    UnsignedIntegralTypes = {
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
        "size_t",
    };
    IntegralTypes = SignedIntegralTypes;
    IntegralTypes.insert(UnsignedIntegralTypes.begin(), UnsignedIntegralTypes.end());

    CppPrimitives = IntegralTypes;
    CppPrimitives.insert("float");
    CppPrimitives.insert("double");
    CppPrimitives.insert("bool");
    CppBasicTypes = CppPrimitives;
    CppBasicTypes.insert("char");
    CppBasicTypes.insert("std::string");
    CppBasicTypes.insert("std::string_view");

    CppIndexedContainers = {
        "std::vector",
        "std::deque",
    };
    CppKeyedContainers = {
        "std::map",
        "std::unordered_map",
    };
    CppSets = {
        "std::set",
        "std::unordered_set",
    };
    CppContainers = CppIndexedContainers;
    CppContainers.insert(CppKeyedContainers.begin(), CppKeyedContainers.end());
    CppContainers.insert(CppSets.begin(), CppSets.end());

    CppStableContainers = {"std::deque"};
    CppStableContainers.insert(CppSets.begin(), CppSets.end());
    CppStableContainers.insert(CppKeyedContainers.begin(), CppKeyedContainers.end());

    KeyableTypes = IntegralTypes;
    KeyableTypes.insert("std::string");
    AllowlistedTypes = {
        "rapidjson::Value",
        "lua_State",
        // TODO: properly validate functions and remove this hack
        "std::function",
        // TODO: support arrays
        // Support array<Elem, Enum> and array<Elem, count>
        "std::array",
        "nullptr_t",
    };
    for (auto&[_, cppType] : TypeToCppType)
      CppTypes.insert(cppType);
  }

  TypeInfo &TypeInfo::Get() {
    static TypeInfo instance;
    return instance;
  }

  std::string Type::ToString() const {
    std::stringstream ss;

    if (mConstexprness == Constexprness::Constexpr)
      ss << "constexpr ";
    if (mConstness == Constness::Const)
      ss << "const ";
    ss << mName;
    if (!mTemplateParameters.empty()) {
      ss << "<";
      bool isFirst = true;
      for (const auto &templateParameter: mTemplateParameters) {
        if (isFirst) {
          isFirst = false;
        } else {
          ss << ", ";
        }
        ss << templateParameter.ToString();
      }
      ss << ">";
    }
    if (!mFunctionalTemplateParameters.empty()) {
      ss << "<";
      for (size_t i = 0; i < mFunctionalTemplateParameters.size(); ++i) {
        if (i == 0) {
          // nothing special
        } else if (i == 1) {
          ss << "(";
        } else {
          ss << ", ";
        }
        ss << mFunctionalTemplateParameters[i].ToString();
      }
      ss << ")>";
    }
    if (mType == PassByType::Reference)
      ss << "&";
    else if (mType == PassByType::Pointer)
      ss << "*";
    else if (mType == PassByType::MoveReference)
      ss << "&&";
    return ss.str();
  }

  Type::Type(
      const ProjectDefinition &project, const TypeDefinition &typeDefinition, PassByType passByType, Constness constness
  ) : mConstness(constness), mType(passByType) {
    if (typeDefinition.mName == "Ref") {
      auto underlyingStruct = project.GetStruct(typeDefinition.mTemplateParameters[0].mName);
      auto idField = underlyingStruct->GetIdField();
      if (idField) {
        *this = Type{project, underlyingStruct->GetIdField()->mType};
      } else {
        *this = Type{underlyingStruct->mName, PassByType::Pointer};
      }
      return;
    }
    auto it = TypeInfo::Get().TypeToCppType.find(typeDefinition.mName);
    if (it != TypeInfo::Get().TypeToCppType.end()) {
      mName = it->second;
    } else {
      mName = typeDefinition.mName;
    }

    for (const auto &templateParameter: typeDefinition.mTemplateParameters) {
      mTemplateParameters.emplace_back(project, templateParameter);
    }
  }

  void Type::PreventCopying(bool addConst) {
    if (!TypeInfo::Get().CppPrimitives.contains(mName)) {
      mType = PassByType::Reference;
      if (addConst)
        mConstness = Constness::Const;
    }
  }
}
