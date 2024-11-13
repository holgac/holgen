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
      {"s8", "int8_t"},
      {"s16", "int16_t"},
      {"s32", "int32_t"},
      {"s64", "int64_t"},
      {"u8", "uint8_t"},
      {"u16", "uint16_t"},
      {"u32", "uint32_t"},
      {"u64", "uint64_t"},
      {"float", "float"},
      {"double", "double"},
      {"bool", "bool"},
      {"string", "std::string"},
      {"vector", "std::vector"},
      {"span", "std::span"},
      {"deque", "std::deque"},
      {"map", "std::map"},
      {"unordered_map", "std::unordered_map"},
      {"set", "std::set"},
      {"unordered_set", "std::unordered_set"},
      {"list", "std::list"},
      {"pair", "std::pair"},
      {"shared_ptr", "std::shared_ptr"},
      {"unique_ptr", "std::unique_ptr"},
  };

  SignedIntegralTypes = {"int8_t",    "int16_t", "int32_t", "int64_t",
                         "ptrdiff_t", "ssize_t", "int",     "char"};
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
  CppSmartPointers = {"std::shared_ptr", "std::unique_ptr"};
  CppBasicTypes = CppPrimitives;
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

namespace {
template <bool FullyQualified>
std::string ToStringGeneric(const Type &type, bool noTrailingSpace, bool ignoreConstForPrimitives,
                            const TranslatedProject *project) {
  std::stringstream ss;

  bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(type.mName);

  if (type.mConstexprness == Constexprness::Constexpr)
    ss << "constexpr ";
  if (type.mConstness == Constness::Const &&
      (!isPrimitive || !ignoreConstForPrimitives || type.mType != PassByType::Value))
    ss << "const ";
  if constexpr (FullyQualified) {
    auto cls = project->GetClass(type.mName);
    if (cls)
      ss << cls->mNamespace << "::";
  }
  ss << type.mName;
  if (!type.mTemplateParameters.empty()) {
    ss << "<";
    bool isFirst = true;
    for (const auto &templateParameter: type.mTemplateParameters) {
      if (isFirst) {
        isFirst = false;
      } else {
        ss << ", ";
      }
      ss << ToStringGeneric<FullyQualified>(templateParameter, true, false, project);
    }
    ss << ">";
  }
  if (!type.mFunctionalTemplateParameters.empty()) {
    ss << "<";
    size_t i = 0;
    for (; i < type.mFunctionalTemplateParameters.size(); ++i) {
      if (i == 0) {
        // nothing special
      } else if (i == 1) {
        ss << "(";
      } else {
        ss << ", ";
      }
      ss << ToStringGeneric<FullyQualified>(type.mFunctionalTemplateParameters[i], true, false,
                                            project);
    }
    if (i == 1)
      ss << "(";
    ss << ")>";
  }
  if (type.mType == PassByType::Reference)
    ss << " &";
  else if (type.mType == PassByType::Pointer) {
    ss << " *";
    for (size_t i = 0; i < type.mPointerDepth; ++i) {
      ss << "*";
    }
  } else if (type.mType == PassByType::MoveReference)
    ss << " &&";
  else if (!noTrailingSpace)
    ss << " ";
  return ss.str();
}
} // namespace

std::string Type::ToString(bool noTrailingSpace, bool ignoreConstForPrimitives) const {
  return ToStringGeneric<false>(*this, noTrailingSpace, ignoreConstForPrimitives, nullptr);
}

std::string Type::ToFullyQualifiedString(const TranslatedProject &project) const {
  return ToStringGeneric<true>(*this, false, false, &project);
}

Type::Type(const TranslatedProject &project, const DefinitionSource &definitionSource,
           const TypeDefinition &typeDefinition, PassByType passByType) :
    Type(project, definitionSource, typeDefinition, passByType, typeDefinition.mConstness) {}

Type::Type(const TranslatedProject &project, const DefinitionSource &definitionSource,
           const TypeDefinition &typeDefinition, PassByType passByType, Constness constness) :
    mConstness(constness), mType(passByType) {
  if (typeDefinition.mName == "Ref") {
    auto underlyingClass = project.GetClass(typeDefinition.mTemplateParameters[0].mName);
    THROW_IF(!underlyingClass, "Class {} referenced in {} does not exist!",
             typeDefinition.mTemplateParameters[0].mName, definitionSource);
    if (auto idField = underlyingClass->GetIdField()) {
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

    switch (typeDefinition.mType) {
    case TypeDefinitionType::Pointer:
      mType = PassByType::Pointer;
      break;
    case TypeDefinitionType::Reference:
      mType = PassByType::Reference;
      break;
    case TypeDefinitionType::Value:
      mType = PassByType::Value;
      break;
    case TypeDefinitionType::Unspecified:
      break;
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

Type Type::ReturnType(const TranslatedProject &project, const FunctionDefinition &func) {
  auto t = Type{project, func.mDefinitionSource, func.mReturnType.mType};
  switch (func.mReturnType.mCategory) {
  case FunctionReturnTypeCategory::Pointer:
    t.mType = PassByType::Pointer;
    break;
  case FunctionReturnTypeCategory::Reference:
    t.mType = PassByType::Reference;
    break;
  case FunctionReturnTypeCategory::NewObject:
    t.mType = PassByType::Value;
    break;
  }
  return t;
}

bool Type::operator==(const Type &rhs) const {
  if (!TypeBase::operator==(rhs)) {
    return false;
  }

  if (mConstness != rhs.mConstness || mType != rhs.mType || mConstexprness != rhs.mConstexprness ||
      mFunctionalTemplateParameters.size() != rhs.mFunctionalTemplateParameters.size() ||
      mPointerDepth != rhs.mPointerDepth)
    return false;
  for (size_t i = 0; i < mFunctionalTemplateParameters.size(); i++) {
    if (mFunctionalTemplateParameters[i] != rhs.mFunctionalTemplateParameters[i])
      return false;
  }
  return true;
}

void Type::PreventCopying(bool addConst) {
  if (!TypeInfo::Get().CppPrimitives.contains(mName)) {
    mType = PassByType::Reference;
    if (addConst)
      mConstness = Constness::Const;
  }
}

bool Type::SupportsCopy(TranslatedProject &project) const {
  std::set<std::string> seenClasses;
  return SupportsCopyOrMirroring(project, seenClasses, true);
}

bool Type::SupportsMirroring(TranslatedProject &project) const {
  std::set<std::string> seenClasses;
  return SupportsCopyOrMirroring(project, seenClasses, false);
}

bool Type::SupportsCopyOrMirroring(TranslatedProject &project, std::set<std::string> &seenClasses,
                                   bool forCopy) const {
  // class A has a vector<A>, which doesn't prevent A from being copyable
  if (seenClasses.contains(mName)) {
    return true;
  }
  if (mName == "std::unique_ptr" || mName == "std::shared_ptr") {
    return false;
  }
  auto cls = project.GetClass(mName);
  if (cls) {
    seenClasses.insert(cls->mName);
    if (cls->mStruct &&
        (cls->mStruct->GetAnnotation(Annotations::DotNetModule) ||
         cls->mStruct->GetAnnotation(Annotations::DotNetInterface))) {
      return false;
    }
    if (forCopy && cls->mStruct &&
        cls->mStruct->GetMatchingAttribute(Annotations::Struct, Annotations::Struct_NonCopyable)) {
      return false;
    }
    for (auto &field: cls->mFields) {
      if (!field.mType.SupportsCopyOrMirroring(project, seenClasses, forCopy)) {
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
    if (!templateParameter.SupportsCopyOrMirroring(project, seenClasses, forCopy)) {
      return false;
    }
  }
  return true;
}

std::string CSharpType::ToString() const {
  std::stringstream ss;
  ss << std::format("{}", mType) << mName;
  for (uint32_t i = 0; i < mArrayDepth; ++i) {
    ss << "[]";
  }
  return ss.str();
}
} // namespace holgen
