#include "TypeInfo.h"
#include <core/Exception.h>
#include <sstream>
#include "TranslatedProject.h"
#include "parser/Parser.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
TypeInfo::TypeInfo() {
  TypeToCppType = {
      {"s8", "int8_t"},          {"s16", "int16_t"},
      {"s32", "int32_t"},        {"s64", "int64_t"},
      {"u8", "uint8_t"},         {"u16", "uint16_t"},
      {"u32", "uint32_t"},       {"u64", "uint64_t"},
      {"float", "float"},        {"double", "double"},
      {"bool", "bool"},          {"string", "std::string"},
      {"vector", "std::vector"}, {"deque", "std::deque"},
      {"map", "std::map"},       {"unordered_map", "std::unordered_map"},
      {"set", "std::set"},       {"unordered_set", "std::unordered_set"},
      {"list", "std::list"},
  };

  SignedIntegralTypes = {
      "int8_t", "int16_t", "int32_t", "int64_t", "ptrdiff_t", "ssize_t", "int",
  };
  UnsignedIntegralTypes = {
      "uint8_t", "uint16_t", "uint32_t", "uint64_t", "size_t",
  };
  FloatingPointTypes = {
      "float",
      "double",
  };
  IntegralTypes = SignedIntegralTypes;
  IntegralTypes.insert(UnsignedIntegralTypes.begin(), UnsignedIntegralTypes.end());

  CppPrimitives = IntegralTypes;
  CppPrimitives.insert(FloatingPointTypes.begin(), FloatingPointTypes.end());
  CppPrimitives.insert("bool");
  CppBasicTypes = CppPrimitives;
  CppBasicTypes.insert("char");
  CppBasicTypes.insert("std::string");
  CppBasicTypes.insert("std::string_view");

  CppFixedSizeContainers = {
      "std::array",
  };
  CppIndexedContainers = {
      "std::vector",
      "std::deque",
      "std::array",
  };
  CppHashContainers = {
      "std::unordered_map",
  };
  CppTreeContainers = {
      "std::map",
  };
  CppKeyedContainers = CppHashContainers;
  CppKeyedContainers.insert(CppTreeContainers.begin(), CppTreeContainers.end());
  CppSets = {
      "std::set",
      "std::unordered_set",
  };
  CppLists = {
      "std::list",
  };
  CppContainers = CppIndexedContainers;
  CppContainers.insert(CppKeyedContainers.begin(), CppKeyedContainers.end());
  CppContainers.insert(CppSets.begin(), CppSets.end());
  CppContainers.insert(CppLists.begin(), CppLists.end());

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
      // TODO: properly validate arrays and remove this hack
      "std::array",
      "std::nullptr_t",
      "std::filesystem::path",
  };
  for (auto &[_, cppType]: TypeToCppType)
    CppTypes.insert(cppType);
}

TypeInfo &TypeInfo::Get() {
  static TypeInfo instance;
  return instance;
}

std::string TypeInfo::GetUnsigned(const std::string &signedType) {
  THROW_IF(!SignedIntegralTypes.contains(signedType), "{} is not a signed integer type",
           signedType);
  if (signedType == "ssize_t" || signedType == "ptrdiff_t")
    return "size_t";
  if (signedType == "int")
    return "uint32_t";
  return "u" + signedType;
}

std::string Type::ToString(bool noTrailingSpace) const {
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
      ss << templateParameter.ToString(true);
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
      ss << mFunctionalTemplateParameters[i].ToString(true);
    }
    ss << ")>";
  }
  if (mType == PassByType::Reference)
    ss << " &";
  else if (mType == PassByType::Pointer)
    ss << " *";
  else if (mType == PassByType::MoveReference)
    ss << " &&";
  else if (!noTrailingSpace)
    ss << " ";
  return ss.str();
}

Type::Type(const TranslatedProject &project, const DefinitionSource &definitionSource,
           const TypeDefinition &typeDefinition, PassByType passByType, Constness constness) :
    mConstness(constness), mType(passByType) {
  if (typeDefinition.mName == "Ref") {
    auto underlyingClass = project.GetClass(typeDefinition.mTemplateParameters[0].mName);
    THROW_IF(!underlyingClass, "Class {} referenced in {} does not exist!",
             typeDefinition.mTemplateParameters[0].mName, definitionSource);
    auto idField = underlyingClass->GetIdField();
    if (idField) {
      *this = idField->mType;
    } else {
      *this = Type{underlyingClass->mStruct->mName, PassByType::Pointer};
    }
  } else {
    auto it = TypeInfo::Get().TypeToCppType.find(typeDefinition.mName);
    if (it != TypeInfo::Get().TypeToCppType.end()) {
      mName = it->second;
    } else {
      mName = typeDefinition.mName;
    }
    for (const auto &templateParameter: typeDefinition.mTemplateParameters) {
      mTemplateParameters.emplace_back(project, definitionSource, templateParameter);
    }
  }

  if (!typeDefinition.mArraySize.empty()) {
    // TODO: validate this - arraySize should either be an enum name or an integer
    decltype(mTemplateParameters) newTemplateParameters;
    newTemplateParameters.emplace_back(*this);
    if (auto e = project.mProject.GetEnum(typeDefinition.mArraySize))
      newTemplateParameters.emplace_back(e->mInvalidValue);
    else
      newTemplateParameters.emplace_back(typeDefinition.mArraySize);
    mName = "std::array";
    mTemplateParameters = std::move(newTemplateParameters);
  }
}

void Type::PreventCopying(bool addConst) {
  if (!TypeInfo::Get().CppPrimitives.contains(mName)) {
    mType = PassByType::Reference;
    if (addConst)
      mConstness = Constness::Const;
  }
}

bool Type::IsCopyable(TranslatedProject &project) const {
  std::set<std::string> seenClasses;
  return IsCopyable(project, seenClasses);
}

bool Type::IsCopyable(TranslatedProject &project, std::set<std::string> &seenClasses) const {
  // class A has a vector<A>, which doesn't prevent A from being copyable
  if (seenClasses.contains(mName)) {
    return true;
  }
  auto cls = project.GetClass(mName);
  if (cls) {
    seenClasses.insert(cls->mName);
    if (cls->mStruct &&
        cls->mStruct->GetMatchingAttribute(Annotations::Struct, Annotations::Struct_NonCopyable)) {
      return false;
    }
    for (auto &field: cls->mFields) {

      if (!field.mType.IsCopyable(project, seenClasses)) {
        return false;
      }
      if (!field.mField) {
        continue;
      }
      if (field.mField->mType.mName == St::Lua_CustomData) {
        return false;
      }
      if (field.mField->mType.mName == St::UserData) {
        auto onCopy =
            field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_OnCopy);
        if (!onCopy || onCopy->mValue.mName != Annotations::Field_OnCopy_Copy) {
          return false;
        }
      }
    }
  }
  for (auto &templateParameter: mTemplateParameters) {
    if (!templateParameter.IsCopyable(project, seenClasses)) {
      return false;
    }
  }
  return true;
}
} // namespace holgen
