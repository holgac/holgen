#include "TypeInfo.h"
#include <sstream>
#include <parser/Parser.h>

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
    };

    SignedIntegralTypes = {
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
    };
    UnsignedIntegralTypes = {
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
    };
    IntegralTypes = SignedIntegralTypes;
    IntegralTypes.insert(UnsignedIntegralTypes.begin(), UnsignedIntegralTypes.end());

    CppPrimitives = IntegralTypes;
    CppPrimitives.insert("float");
    CppPrimitives.insert("double");
    CppPrimitives.insert("bool");

    CppIndexedContainers = {
        "std::vector",
        "std::deque",
    };
    CppKeyedContainers = {
        "std::map",
        "std::unordered_map",
    };
    CppContainers = CppIndexedContainers;
    CppContainers.insert(CppKeyedContainers.begin(), CppKeyedContainers.end());
    CppStableContainers = {
        "std::deque",
    };
    KeyableTypes = IntegralTypes;
    KeyableTypes.insert("std::string");
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
      const TypeDefinition &typeDefinition, PassByType passByType, Constness constness
  ) : mConstness(constness), mType(passByType) {
    auto it = TypeInfo::Get().TypeToCppType.find(typeDefinition.mName);
    if (it != TypeInfo::Get().TypeToCppType.end()) {
      mName = it->second;
    } else {
      mName = typeDefinition.mName;
    }

    for (const auto &templateParameter: typeDefinition.mTemplateParameters) {
      mTemplateParameters.emplace_back(templateParameter);
    }
  }
}