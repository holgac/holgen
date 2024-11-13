#include "EnumPlugin.h"
#include <format>
#include <string>
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
void EnumPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mEnum == nullptr || cls.mEnum->mType != EnumDefinitionType::Enum)
      continue;
    GenerateUnderlyingType(cls, DetermineUnderlyingType(cls));
    GenerateClassEnum(cls);
    GenerateValueField(cls, std::format("{}::Entry", cls.mName));
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
    GenerateProperties(cls);
    GenerateFormatter(cls, true);
    GenerateFormatter(cls, false);
    GenerateXMacros(cls);
    Validate().Enum(cls);
  }
}

void EnumPlugin::GenerateEnumConstructor(Class &cls) {
  auto ctor = ClassConstructor{};
  ctor.mArguments.emplace_back("value", Type{"Entry"});
  ctor.mInitializerList.emplace_back("mValue", "value");
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

void EnumPlugin::GenerateProperties(Class &cls) {
  for (auto &annotation: cls.mEnum->GetAnnotations(Annotations::EnumProperty)) {
    GenerateProperty(cls, annotation);
  }
}

void EnumPlugin::GenerateProperty(Class &cls, const AnnotationDefinition &annotation) {
  Validate().EnumPropertyAnnotation(cls, annotation);
  auto typeAttribute = annotation.GetAttribute(Annotations::EnumProperty_Type);
  auto &defaultValue = annotation.GetAttribute(Annotations::EnumProperty_Default)->mValue.mName;
  auto &name = annotation.GetAttribute(Annotations::EnumProperty_Name)->mValue.mName;
  std::map<std::string, std::vector<const EnumEntryDefinition *>> valueToEntries;
  for (auto &entry: cls.mEnum->mEntries) {
    auto entryAnnotation = entry.GetMatchingAnnotation(Annotations::EnumProperty,
                                                       Annotations::EnumProperty_Name, name);
    const std::string *value;
    if (entryAnnotation) {
      value = &entryAnnotation->GetAttribute(Annotations::EnumProperty_Value)->mValue.mName;
    } else {
      value = &defaultValue;
    }
    valueToEntries[*value].push_back(&entry);
  }

  auto method =
      ClassMethod{Naming().FieldGetterNameInCpp(name),
                  Type{mProject, typeAttribute->mDefinitionSource, typeAttribute->mValue}};
  bool addQuotes = false;
  if (typeAttribute->mValue.mName == "string") {
    addQuotes = true;
    method.mReturnType = Type{"char", PassByType::Pointer, Constness::Const};
  } else if (!mProject.mProject.GetEnum(typeAttribute->mValue.mName)) {
    method.mReturnType.PreventCopying();
  }

  method.mBody.Add("switch (mValue) {{");
  for (auto &[value, entries]: valueToEntries) {
    for (auto &entry: entries) {
      method.mBody.Add("case Entry::{}:", entry->mName);
    }
    method.mBody.Indent(1);
    if (addQuotes) {
      method.mBody.Add("return \"{}\";", value);
    } else {
      method.mBody.Add("return {};", value);
    }
    method.mBody.Indent(-1);
  }
  method.mBody.Add("}}");
  if (addQuotes) {
    method.mBody.Add("return \"{}\";", defaultValue);
  } else {
    method.mBody.Add("return {};", defaultValue);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPlugin::GenerateXMacros(Class &cls) {
  {
    auto macro = Macro{Naming().EnumClassForEachMacro(cls)};
    for (auto &entry: cls.mEnum->mEntries) {
      macro.mBody.Add("{}({})", Naming().EnumClassForEachDoerMacro(cls), entry.mName);
    }
    cls.mHeaderMacros.push_back(std::move(macro));
  }
  {
    auto macro = Macro{Naming().EnumClassSwitchMacro(cls)};
    macro.mArguments.emplace_back("VALUE");
    macro.mBody.Add("switch ((VALUE)) {{");
    std::string namespaceSuffix;
    if (!cls.mNamespace.empty())
      namespaceSuffix = cls.mNamespace + "::";
    for (auto &entry: cls.mEnum->mEntries) {
      macro.mBody.Add("case {}{}::{}:", namespaceSuffix, cls.mName, entry.mName);
      macro.mBody.Indent(1);
      macro.mBody.Add("{}({})", Naming().EnumClassForEachDoerMacro(cls), entry.mName);
      macro.mBody.Indent(-1);
    }
    macro.mBody.Add("}}");
    cls.mHeaderMacros.push_back(std::move(macro));
  }
}

std::string EnumPlugin::DetermineUnderlyingType(Class &cls) {
  auto typeAttribute = cls.mEnum->GetMatchingAttribute(Annotations::Enum, Annotations::Enum_Type);
  if (typeAttribute) {
    return Type{mProject, typeAttribute->mDefinitionSource, typeAttribute->mValue}.mName;
  }
  if (cls.mEnum->mEntries.empty()) {
    return "uint8_t";
  }
  uint64_t largest = 0;
  for (auto &entry: cls.mEnum->mEntries) {
    if (entry.mValue[0] == '-') {
      return "int64_t";
    }
    uint64_t val;
    try {
      val = std::stoull(entry.mValue);
    } catch (const std::invalid_argument &) {
      return "int64_t";
    }
    largest = std::max(val, largest);
  }
  if (largest < 0x100ull)
    return "uint8_t";
  if (largest < 0x10000ull)
    return "uint16_t";
  if (largest < 0x100000000ull)
    return "uint32_t";
  return "uint64_t";
}

} // namespace holgen
