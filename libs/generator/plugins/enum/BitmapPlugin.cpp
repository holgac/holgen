#include "BitmapPlugin.h"

#include "holgen.h"

// TODO: This class is tested in blackbox only; write some unit tests too

namespace holgen {
void BitmapPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mEnum == nullptr || cls.mEnum->mType != EnumDefinitionType::Bitmap)
      continue;
    GenerateUnderlyingType(cls);
    GenerateValueField(cls);
    GenerateClassEnum(cls, "Index");
    GenerateClassEnumForEntryValues(cls);
    GenerateIntegralConstructor(cls, "0");
    GenerateGetValue(cls);
    GenerateFromStringSingle(cls, "FromStringSingle", Visibility::Private);
    GenerateFromString(cls);
    GenerateToStringSingle(cls, "ToStringSingle", Visibility::Private);
    GenerateToString(cls);
    GenerateOperators(cls);
    GenerateBitmapOperators(cls);
    GenerateHasMethod(cls, true);
    GenerateHasMethod(cls, false);
    GenerateGetEntries(cls, "GetEntries", "");
    GenerateGetEntries(cls, "GetEntryIndices", "Index");
    GenerateFormatter(cls, true);
    GenerateFormatter(cls, false);
    Validate().Enum(cls);
  }
}

void BitmapPlugin::GenerateHasMethod(Class &cls, bool forNestedEnum) {
  auto method = ClassMethod{"Has", Type{"bool"}};
  method.mArguments.emplace_back(
      "val", Type{forNestedEnum ? cls.mName : std::format("{}::Entry", cls.mName)});
  method.mArguments.front().mType.PreventCopying();
  if (forNestedEnum)
    method.mBody.Add("return (mValue & val.mValue) == val.mValue;");
  else
    method.mBody.Add("return (mValue & val) == val;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void BitmapPlugin::GenerateClassEnumForEntryValues(Class &cls) {
  auto classEnum = ClassEnum{"Entry", "UnderlyingType"};
  for (auto &entry: cls.mEnum->mEntries) {
    auto entryBitValue = strtoll(entry.mValue.data(), nullptr, 10);
    auto entryValue = 1 << entryBitValue;
    auto classEnumEntry = ClassEnumEntry{entry.mName, std::to_string(entryValue)};
    classEnumEntry.mEntry = &entry;
    Validate().NewEnumEntry(cls, classEnum, classEnumEntry);
    classEnum.mEntries.push_back(std::move(classEnumEntry));
  }
  cls.mNestedEnums.emplace_back(std::move(classEnum));
}

void BitmapPlugin::GenerateGetValue(Class &cls) {
  auto method = ClassMethod{"GetValue", Type{"UnderlyingType"}};
  method.mBody.Add("return mValue;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void BitmapPlugin::GenerateFromString(Class &cls) {
  auto method = ClassMethod{"FromString", Type{cls.mName}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mArguments.emplace_back("str", Type{"std::string_view"});
  method.mBody.Add("{} result;", cls.mName);
  method.mBody.Add("size_t curIdx = 0, commaIdx = str.find(',');");
  method.mBody.Add("while (commaIdx != std::string::npos) {{");
  method.mBody.Indent(1);
  method.mBody.Add("result |= {}::FromStringSingle(str.substr(curIdx, commaIdx));", cls.mName);
  method.mBody.Add("curIdx = commaIdx + 1;");
  method.mBody.Add("commaIdx = str.find(',', curIdx);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("result |= {}::FromStringSingle(str.substr(curIdx));", cls.mName);
  method.mBody.Add("return result;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void BitmapPlugin::GenerateToString(Class &cls) {
  // TODO: check all values and construct a string
  cls.mSourceIncludes.AddStandardHeader("sstream");
  auto method = ClassMethod{"ToString", Type{"std::string"}, Visibility::Public, Constness::Const};
  method.mBody.Add("std::stringstream ss;");
  method.mBody.Add("bool isFirst = true;");
  method.mBody.Add("for (auto& entry: GetEntries()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if (Has(entry)) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if (isFirst) {{");
  method.mBody.Indent(1);
  method.mBody.Add("isFirst = false;");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add("ss << \",\";");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("ss << {}(entry).ToStringSingle();", cls.mName);
  method.mBody.Indent(-1);
  method.mBody.Add("}}");

  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("return ss.str();");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void BitmapPlugin::GenerateBitmapOperators(Class &cls) {
  const std::vector<EnumOperator> operators = {
      {"|=", Constness::NotConst, EnumOperatorReturnType::This, EnumOperatorArgumentType::Entry},
      {"|=", Constness::NotConst, EnumOperatorReturnType::This, EnumOperatorArgumentType::Class},
      {"|", Constness::Const, EnumOperatorReturnType::Result, EnumOperatorArgumentType::Class}};

  for (auto &op: operators) {
    GenerateOperator(cls, op);
  }
}
} // namespace holgen
