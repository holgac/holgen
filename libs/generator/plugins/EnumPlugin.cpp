#include "EnumPlugin.h"
#include <format>
#include "core/St.h"

namespace holgen {
void EnumPlugin::Run() {

  for (auto &cls: mProject.mClasses) {
    if (cls.mEnum == nullptr)
      continue;
    GenerateCommon(cls);
    Validate().Enum(cls);
  }
}

namespace {
enum class EnumOperatorReturnType { This, Result };
struct EnumOperator {
  std::string mOperator;
  Constness mConstness;
  EnumOperatorReturnType mReturn;
  bool mIntegralArgument;
};
} // namespace

void EnumPlugin::GenerateCommon(Class &cls) {
  cls.mUsings.emplace_back(Type{"int64_t"}, St::Enum_UnderlyingType);

  {
    auto field = ClassField{"mValue", Type{St::Enum_UnderlyingType}};
    Validate().NewField(cls, field);
    cls.mFields.push_back(std::move(field));
  }

  GenerateClassEnum(cls);
  GenerateIntegralConstructor(cls);
  GenerateEnumConstructor(cls);
  GenerateGetValue(cls);
  GenerateInvalidEntry(cls);
  GenerateFromString(cls);
  GenerateToString(cls);
  GenerateOperators(cls);
  GenerateGetEntries(cls);
  GenerateHash(cls);
  GenerateFormatter(cls, true);
  GenerateFormatter(cls, false);
  // TODO: should export to lua (probably in lua plugin instead of here)
}

void EnumPlugin::GenerateIntegralConstructor(Class &cls) {
  auto ctor = ClassConstructor{};
  ctor.mExplicitness = Explicitness::Explicit;
  ctor.mArguments.emplace_back("value", Type{St::Enum_UnderlyingType}, "Invalid");
  ctor.mInitializerList.emplace_back("mValue", "value");
  // TODO: validate ctors
  cls.mConstructors.push_back(std::move(ctor));
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

void EnumPlugin::GenerateOperators(Class &cls) {
  // TODO: test these properly. Currently overloads aren't well supported.
  const std::vector<EnumOperator> operators = {{"=", Constness::NotConst, EnumOperatorReturnType::This, true},
                                               {"==", Constness::Const, EnumOperatorReturnType::Result, true},
                                               {"==", Constness::Const, EnumOperatorReturnType::Result, false},
                                               {"!=", Constness::Const, EnumOperatorReturnType::Result, true},
                                               {"!=", Constness::Const, EnumOperatorReturnType::Result, false},
                                               {"<", Constness::Const, EnumOperatorReturnType::Result, true},
                                               {"<", Constness::Const, EnumOperatorReturnType::Result, false}};

  for (auto &op: operators) {
    auto method = ClassMethod{"operator" + op.mOperator, Type{"bool"}, Visibility::Public, op.mConstness};
    if (op.mReturn == EnumOperatorReturnType::This) {
      method.mReturnType.mName = cls.mName;
      method.mReturnType.mType = PassByType::Reference;
    }
    if (op.mIntegralArgument)
      method.mArguments.emplace_back("rhs", Type{St::Enum_UnderlyingType});
    else
      method.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
    {
      auto line = method.mBody.Line();
      if (op.mReturn == EnumOperatorReturnType::Result)
        line << "return ";
      line << "mValue";
      line << " " << op.mOperator << " rhs";
      if (!op.mIntegralArgument)
        line << ".mValue";
      line << ";";
    }
    if (op.mReturn == EnumOperatorReturnType::This)
      method.mBody.Add("return *this;");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void EnumPlugin::GenerateGetEntries(Class &cls) {
  auto method =
      ClassMethod{"GetEntries", Type{"std::array"}, Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mConstexprness = Constexprness::Constexpr;
  method.mReturnType.mTemplateParameters.emplace_back(std::format("{}::Entry", cls.mName));
  method.mReturnType.mTemplateParameters.emplace_back(std::format("{}", cls.mEnum->mEntries.size()));
  {
    auto line = method.mBody.Line();
    line << "return " << method.mReturnType.ToString(true) << "{";
    for (size_t i = 0; i < cls.mEnum->mEntries.size(); ++i) {
      if (i > 0)
        line << ", ";
      line << cls.mEnum->mEntries[i].mName;
    }
    line << "};";
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPlugin::GenerateInvalidEntry(Class &cls) {
  auto invalidEntry = ClassField{"Invalid", Type{St::Enum_UnderlyingType, PassByType::Value, Constness::Const},
                                 Visibility::Public, Staticness::Static, cls.mEnum->mInvalidValue};
  invalidEntry.mType.mConstexprness = Constexprness::Constexpr;
  invalidEntry.mDefaultValue = cls.mEnum->mInvalidValue;
  Validate().NewField(cls, invalidEntry);
  cls.mFields.push_back(std::move(invalidEntry));
}

void EnumPlugin::GenerateFromString(Class &cls) {
  auto method = ClassMethod{"FromString", Type{cls.mName}, Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("str", Type{"std::string_view"});
  bool isFirst = true;
  for (auto &entry: cls.mEnum->mEntries) {
    if (isFirst) {
      isFirst = false;
      method.mBody.Add("if (str == \"{}\") {{", entry.mName);
    } else {
      method.mBody.Add("}} else if (str == \"{}\") {{", entry.mName);
    }
    method.mBody.Indent(1);
    method.mBody.Add("return {}({});", cls.mName, entry.mValue);
    method.mBody.Indent(-1);
  }
  if (!cls.mEnum->mEntries.empty()) {
    method.mBody.Add("}} else {{");
    method.mBody.Indent(1);
  }
  method.mBody.Add("return {}({}::Invalid);", cls.mName, cls.mName);
  if (!cls.mEnum->mEntries.empty()) {
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPlugin::GenerateToString(Class &cls) {
  // TODO: << operator?
  auto method = ClassMethod{"ToString", Type{"char", PassByType::Pointer, Constness::Const}, Visibility::Public,
                            Constness::Const};
  method.mBody.Add("switch (mValue) {{");
  method.mBody.Indent(1);
  for (auto &entry: cls.mEnum->mEntries) {
    method.mBody.Add("case {}:", entry.mValue);
    method.mBody.Indent(1);
    method.mBody.Add("return \"{}\";", entry.mName);
    method.mBody.Indent(-1);
  }
  method.mBody.Add("default:");
  method.mBody.Indent(1);
  method.mBody.Add("return \"INVALID\";");
  method.mBody.Indent(-1);
  method.mBody.Indent(-1);
  method.mBody.Add("}}"); // switch
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
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

void EnumPlugin::GenerateFormatter(Class &cls, bool forNestedEnum) {
  cls.mHeaderIncludes.AddStandardHeader("format");
  auto className = std::format("{}::{}", cls.mNamespace, cls.mName);
  auto formatter = Class{"formatter", "std"};
  if (forNestedEnum)
    formatter.mTemplateSpecializations.push_back(className + "::Entry");
  else
    formatter.mTemplateSpecializations.push_back(className);
  formatter.mClassType = ClassType::Struct;
  formatter.mBaseClasses.push_back("formatter<string>");

  auto format = ClassMethod{"format", Type{"auto"}, Visibility::Public, Constness::Const};
  format.mTemplateParameters.emplace_back("typename", "FormatContext");
  format.mArguments.emplace_back("obj", Type{className, PassByType::Reference, Constness::Const});
  format.mArguments.emplace_back("ctx", Type{"FormatContext", PassByType::Reference, Constness::NotConst});
  if (forNestedEnum)
    format.mBody.Add("return format_to(ctx.out(), \"{{}}\", {}(obj).ToString());", className);
  else
    format.mBody.Add("return format_to(ctx.out(), \"{{}}\", obj.ToString());");
  formatter.mMethods.push_back(std::move(format));
  cls.mSpecializations.push_back(std::move(formatter));
}

void EnumPlugin::GenerateClassEnum(Class &cls) {
  auto classEnum = ClassEnum{"Entry", "UnderlyingType"};
  for (auto &entry: cls.mEnum->mEntries) {
    auto classEnumEntry = ClassEnumEntry{entry.mName, entry.mValue};
    classEnumEntry.mEntry = &entry;
    Validate().NewEnumEntry(cls, classEnum, classEnumEntry);
    classEnum.mEntries.push_back(std::move(classEnumEntry));
  }
  cls.mNestedEnums.emplace_back(std::move(classEnum));
}
} // namespace holgen
