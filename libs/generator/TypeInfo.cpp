#include "TypeInfo.h"
#include <sstream>
#include <parser/Parser.h>

namespace holgen {
  TypeInfo::TypeInfo() {
    TypeToCppType = {
        {"s8",      "int8_t"},
        {"s16",     "int16_t"},
        {"s32",     "int32_t"},
        {"s64",     "int64_t"},
        {"u8",      "uint8_t"},
        {"u16",     "uint16_t"},
        {"u32",     "uint32_t"},
        {"u64",     "uint64_t"},
        {"float",   "float"},
        {"double",  "double"},
        {"string",  "std::string"},
        {"vector",  "std::vector"},
        {"map",     "std::map"},
        {"unordered_map", "std::unordered_map"},
    };

    CppPrimitives = {
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
        "float",
        "double",
        "bool",
    };
    CppIndexedContainers = {
        "std::vector",
    };
    CppKeyedContainers = {
        "std::map",
        "std::unordered_map",
    };
  }

  TypeInfo &TypeInfo::Get() {
    static TypeInfo instance;
    return instance;
  }

  void TypeInfo::ConvertToType(
      Type &type,
      const TypeDefinition &typeDefinition
  ) {
    auto it = TypeInfo::Get().TypeToCppType.find(typeDefinition.mName);
    if (it != TypeInfo::Get().TypeToCppType.end()) {
      type.mName = it->second;
    } else {
      type.mName = typeDefinition.mName;
    }

    for (const auto &templateParameter: typeDefinition.mTemplateParameters) {
      ConvertToType(type.mTemplateParameters.emplace_back(), templateParameter);
    }
  }


  std::string Type::ToString() const {
    std::stringstream ss;

    if (mIsConst)
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
      for(size_t i =0; i<mFunctionalTemplateParameters.size(); ++i) {
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
}