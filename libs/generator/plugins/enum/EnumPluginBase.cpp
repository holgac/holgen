#include "EnumPluginBase.h"
#include "core/St.h"

namespace holgen {

void EnumPluginBase::GenerateIntegralConstructor(Class &cls, const std::string &defaultValue) {
  auto ctor = ClassConstructor{};
  ctor.mExplicitness = Explicitness::Explicit;
  ctor.mArguments.emplace_back("value", Type{St::Enum_UnderlyingType}, defaultValue);
  auto valueField = cls.GetField("mValue");
  ctor.mInitializerList.emplace_back("mValue", std::format("{}(value)", valueField->mType.mName));
  // TODO: validate ctors
  cls.mConstructors.push_back(std::move(ctor));
}

void EnumPluginBase::GenerateUnderlyingType(Class &cls, const std::string &type) {
  cls.mUsings.emplace_back(Type{type}, St::Enum_UnderlyingType);
}

void EnumPluginBase::GenerateValueField(Class &cls, const std::string &type) {
  auto field = ClassField{"mValue", Type{type}};
  Validate().NewField(cls, field);
  cls.mFields.push_back(std::move(field));
}

void EnumPluginBase::GenerateClassEnum(Class &cls, const std::string &entrySuffix) {
  auto classEnum = ClassEnum{"Entry" + entrySuffix, "UnderlyingType"};
  for (auto &entry: cls.mEnum->mEntries) {
    auto classEnumEntry = ClassEnumEntry{entry.mName + entrySuffix, entry.mValue};
    classEnumEntry.mEntry = &entry;
    FillComments(entry, classEnumEntry.mComments);
    Validate().NewEnumEntry(cls, classEnum, classEnumEntry);
    classEnum.mEntries.push_back(std::move(classEnumEntry));
  }
  cls.mNestedEnums.emplace_back(std::move(classEnum));
}

void EnumPluginBase::GenerateFromStringSingle(Class &cls, const std::string &methodName,
                                              Visibility visibility,
                                              const std::string &valueOnFailure) {
  auto method =
      ClassMethod{methodName, Type{cls.mName}, visibility, Constness::NotConst, Staticness::Static};
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
    method.mBody.Add("return {0}({0}::{1});", cls.mName, entry.mName);
    method.mBody.Indent(-1);
  }
  if (!cls.mEnum->mEntries.empty()) {
    method.mBody.Add("}} else {{");
    method.mBody.Indent(1);
  }
  method.mBody.Add("HOLGEN_WARN(\"{{}} is not a valid {}, returning invalid\", str);", cls.mName);
  method.mBody.Add("return {}{{{}}};", cls.mName, valueOnFailure);
  if (!cls.mEnum->mEntries.empty()) {
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPluginBase::GenerateToStringSingle(Class &cls, const std::string &methodName,
                                            Visibility visibility) {
  // TODO: << operator?
  auto method = ClassMethod{methodName, Type{"char", PassByType::Pointer, Constness::Const},
                            visibility, Constness::Const};
  method.mBody.Add("switch (mValue) {{");
  method.mBody.Indent(1);
  for (auto &entry: cls.mEnum->mEntries) {
    method.mBody.Add("case {}::{}:", cls.mName, entry.mName);
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

void EnumPluginBase::GenerateOperator(Class &cls, const EnumOperator &op) {
  auto method =
      ClassMethod{"operator" + op.mOperator, Type{"bool"}, Visibility::Public, op.mConstness};
  if (op.mReturn == EnumOperatorReturnType::This) {
    method.mReturnType.mName = cls.mName;
    method.mReturnType.mType = PassByType::Reference;
  }
  auto valueField = cls.GetField("mValue");
  switch (op.mArgumentType) {
  case EnumOperatorArgumentType::UnderlyingType:
    method.mArguments.emplace_back("rhs", Type{St::Enum_UnderlyingType});
    break;
  case EnumOperatorArgumentType::Class:
    method.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
    break;
  case EnumOperatorArgumentType::Entry:
    method.mArguments.emplace_back("rhs", Type{std::format("{}::Entry", cls.mName)});
    break;
  }
  {
    auto line = method.mBody.Line();
    if (op.mReturn == EnumOperatorReturnType::Result)
      line << "return ";
    line << "mValue " << op.mOperator << " ";
    // TODO: put "UnderlyingType" in St.
    bool needsCast = op.mArgumentType == EnumOperatorArgumentType::UnderlyingType &&
        valueField->mType.mName != "UnderlyingType";
    if (needsCast)
      line << valueField->mType.mName << "(";

    line << "rhs";
    if (op.mArgumentType == EnumOperatorArgumentType::Class)
      line << ".mValue";
    if (needsCast)
      line << ")";
    line << ";";
  }
  if (op.mReturn == EnumOperatorReturnType::This)
    method.mBody.Add("return *this;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPluginBase::GenerateOperators(Class &cls) {
  // TODO: test these properly. Currently overloads aren't well supported.
  const std::vector<EnumOperator> operators = {
      {"=", Constness::NotConst, EnumOperatorReturnType::This,
       EnumOperatorArgumentType::UnderlyingType},
      {"==", Constness::Const, EnumOperatorReturnType::Result,
       EnumOperatorArgumentType::UnderlyingType},
      {"==", Constness::Const, EnumOperatorReturnType::Result, EnumOperatorArgumentType::Class},
      {"!=", Constness::Const, EnumOperatorReturnType::Result,
       EnumOperatorArgumentType::UnderlyingType},
      {"!=", Constness::Const, EnumOperatorReturnType::Result, EnumOperatorArgumentType::Class},
      {"<", Constness::Const, EnumOperatorReturnType::Result,
       EnumOperatorArgumentType::UnderlyingType},
      {"<", Constness::Const, EnumOperatorReturnType::Result, EnumOperatorArgumentType::Class}};

  for (auto &op: operators) {
    GenerateOperator(cls, op);
  }
}

void EnumPluginBase::GenerateGetEntries(Class &cls, const std::string &methodName,
                                        const std::string &entrySuffix) {
  auto method = ClassMethod{methodName, Type{"std::array"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mConstexprness = Constexprness::Constexpr;
  method.mReturnType.mTemplateParameters.emplace_back(
      std::format("{}::Entry{}", cls.mName, entrySuffix));
  method.mReturnType.mTemplateParameters.emplace_back(
      std::format("{}", cls.mEnum->mEntries.size()));
  {
    auto line = method.mBody.Line();
    line << "return " << method.mReturnType.ToString(true) << "{";
    for (size_t i = 0; i < cls.mEnum->mEntries.size(); ++i) {
      if (i > 0)
        line << ", ";
      line << cls.mEnum->mEntries[i].mName << entrySuffix;
    }
    line << "};";
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void EnumPluginBase::GenerateFormatter(Class &cls, bool forNestedEnum) {
  cls.mHeaderIncludes.AddStandardHeader("format");
  auto className = std::format("{}::{}", cls.mNamespace, cls.mName);
  auto formatter = Class{"formatter", "std"};
  if (forNestedEnum)
    formatter.mTemplateSpecializations.push_back(className + "::Entry");
  else
    formatter.mTemplateSpecializations.push_back(className);
  formatter.mClassType = ClassType::Struct;

  auto &base = formatter.mBaseClasses.emplace_back(Visibility::Public, Type{"formatter"});
  base.mType.mTemplateParameters.emplace_back("string");

  auto format = ClassMethod{"format", Type{"auto"}, Visibility::Public, Constness::Const};
  format.mTemplateParameters.emplace_back("typename", "FormatContext");
  if (forNestedEnum)
    format.mArguments.emplace_back(
        "obj", Type{std::format("{}::Entry", className), PassByType::Reference, Constness::Const});
  else
    format.mArguments.emplace_back("obj", Type{className, PassByType::Reference, Constness::Const});
  format.mArguments.emplace_back("ctx",
                                 Type{"FormatContext", PassByType::Reference, Constness::NotConst});
  if (forNestedEnum)
    format.mBody.Add("return format_to(ctx.out(), \"{{}}\", {}(obj).ToString());", className);
  else
    format.mBody.Add("return format_to(ctx.out(), \"{{}}\", obj.ToString());");
  formatter.mMethods.push_back(std::move(format));
  cls.mSpecializations.push_back(std::move(formatter));
}

} // namespace holgen
