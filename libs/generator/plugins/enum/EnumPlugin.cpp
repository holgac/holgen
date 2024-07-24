#include "EnumPlugin.h"
#include <format>
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
void EnumPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mEnum == nullptr || cls.mEnum->mType != EnumDefinitionType::Enum)
      continue;
    GenerateUnderlyingType(cls);
    GenerateValueField(cls);
    GenerateClassEnum(cls);
    GenerateIntegralConstructor(cls);
    GenerateEnumConstructor(cls);
    GenerateGetValue(cls);
    GenerateInvalidEntry(cls);
    GenerateFromStringSingle(cls, "FromString", Visibility::Public,
                             std::format("{}::Invalid", cls.mName));
    GenerateToStringSingle(cls, "ToString", Visibility::Public);
    GenerateOperators(cls);
    GenerateGetEntries(cls, "GetEntries", "");
    GenerateHash(cls);
    GenerateFormatter(cls, true);
    GenerateFormatter(cls, false);
    Validate().Enum(cls);
  }
}

void EnumPlugin::GenerateEnumConstructor(Class &cls) {
  auto ctor = ClassConstructor{};
  ctor.mArguments.emplace_back("value", Type{"Entry"});
  ctor.mInitializerList.emplace_back("mValue", "UnderlyingType(value)");
  // TODO: validate ctors
  cls.mConstructors.push_back(std::move(ctor));
}

void EnumPlugin::GenerateGetValue(Class &cls) {
  auto method = ClassMethod{"GetValue", Type{cls.mName + "::Entry"}};
  method.mBody.Add("return Entry(mValue);");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPlugin::GenerateInvalidEntry(Class &cls) {
  auto invalidEntry =
      ClassField{"Invalid", Type{St::Enum_UnderlyingType, PassByType::Value, Constness::Const},
                 Visibility::Public, Staticness::Static, cls.mEnum->mInvalidValue};
  invalidEntry.mType.mConstexprness = Constexprness::Constexpr;
  invalidEntry.mDefaultValue = cls.mEnum->mInvalidValue;
  Validate().NewField(cls, invalidEntry);
  cls.mFields.push_back(std::move(invalidEntry));
}

void EnumPlugin::GenerateHash(Class &cls) {
  auto className = std::format("{}::{}", cls.mNamespace, cls.mName);
  auto hash = Class{"hash", "std"};
  hash.mTemplateSpecializations.push_back(className);
  hash.mClassType = ClassType::Struct;

  auto hasher = ClassMethod{"operator()", Type{"size_t"}, Visibility::Public, Constness::Const};
  hasher.mArguments.emplace_back("obj", Type{className, PassByType::Reference, Constness::Const});
  hasher.mBody.Add("return std::hash<{}::UnderlyingType>()(obj.GetValue());", className);
  hash.mMethods.push_back(std::move(hasher));
  cls.mSpecializations.push_back(std::move(hash));
}

void EnumPlugin::GenerateIntegralConstructor(Class &cls) {
  if (auto defaultEntry = cls.mEnum->GetDefaultEntry()) {
    EnumPluginBase::GenerateIntegralConstructor(cls, std::format("Entry::{}", defaultEntry->mName));
  } else {
    EnumPluginBase::GenerateIntegralConstructor(cls, "Invalid");
  }
}

} // namespace holgen
