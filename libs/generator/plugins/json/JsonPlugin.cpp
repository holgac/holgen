#include "JsonPlugin.h"
#include <vector>
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/StringSwitcher.h"
#include "generator/TypeInfo.h"

namespace holgen {
namespace {
// To represent structs with a single (non-id) field in json,
// we might want to just use that field instead of a json object.
// This only works when the field type is primitive, string or a sequence
ClassField *GetSingleBasicField(Class &cls) {
  ClassField *singleField = nullptr;
  ClassField *idField = nullptr;
  for (auto &field: cls.mFields) {
    if (!field.mField)
      continue;
    if (field.mField->GetAnnotation(Annotations::Id)) {
      idField = &field;
      continue;
    }
    if (singleField)
      return nullptr;
    singleField = &field;
  }
  if (singleField) {
    if (TypeInfo::Get().CppBasicTypes.contains(singleField->mType.mName) ||
        TypeInfo::Get().CppIndexedContainers.contains(singleField->mType.mName) ||
        TypeInfo::Get().CppSets.contains(singleField->mType.mName))
      return singleField;
    return nullptr;
  }
  // id field is always basic
  return idField;
}
} // namespace

void JsonPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct)
      ProcessStruct(cls);
    else if (cls.mEnum)
      ProcessEnum(cls);
  }
}

void JsonPlugin::GenerateSwitcherLoop(ClassMethod &method, CodeBlock &&codeBlock) {
  method.mBody.Add("for (const auto &data: json.GetObject()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("const auto &name = data.name.GetString();");
  method.mBody.Add(std::move(codeBlock));
  method.mBody.Indent(-1);
  method.mBody.Line() << "}"; // range based for on json.GetObject()
}

void JsonPlugin::GenerateParseJson(Class &cls) {
  auto method = ClassMethod{St::ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});

  StringSwitcher variantSwitcher("name", {});

  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(
      R"R(HOLGEN_WARN("Unexpected entry in json when parsing {}: {{}}", name);)R", cls.mName);
  StringSwitcher switcher("name", std::move(stringSwitcherElseCase));
  for (const auto &field: cls.mFields) {
    if (field.mField &&
        (field.mField->GetAnnotation(Annotations::NoJson) ||
         field.mField->mType.mName == St::UserData))
      continue;
    const std::string *variantRawName = nullptr;
    bool isVariantTypeField = IsVariantTypeField(cls, field, &variantRawName);
    if (!field.mField && !isVariantTypeField)
      continue;
    if (isVariantTypeField) {
      variantSwitcher.AddCase(field.mField ? field.mField->mName : *variantRawName,
                              [&](CodeBlock &switchBlock) {
                                GenerateParseJsonForField(cls, switchBlock, field, "data.value");
                              });
      switcher.AddCase(field.mField ? field.mField->mName : *variantRawName,
                       [&](CodeBlock &switchBlock HOLGEN_ATTRIBUTE_UNUSED) {});
    } else {
      switcher.AddCase(field.mField ? field.mField->mName : *variantRawName,
                       [&](CodeBlock &switchBlock) {
                         GenerateParseJsonForField(cls, switchBlock, field, "data.value");
                       });
    }
  }

  for (const auto &luaMethod: cls.mMethods) {
    if (!luaMethod.mFunction || luaMethod.mFunction->GetAnnotation(Annotations::NoJson) ||
        !luaMethod.mFunction->GetAnnotation(Annotations::LuaFunc))
      continue;
    switcher.AddCase(luaMethod.mFunction->mName, [&](CodeBlock &switchBlock) {
      GenerateParseJsonForFunction(cls, switchBlock, luaMethod);
    });
  }

  if (!variantSwitcher.IsEmpty()) {
    GenerateSwitcherLoop(method, std::move(variantSwitcher.Generate()));
  }

  if (!switcher.IsEmpty()) {
    auto singleField = GetSingleBasicField(cls);
    if (singleField) {
      method.mBody.Add("if (json.IsObject()) {{");
      method.mBody.Indent(1);
    } else {
      method.mBody.Add(
          R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing {}");)R",
          cls.mName);
    }

    GenerateSwitcherLoop(method, std::move(switcher.Generate()));
    if (singleField) {
      method.mBody.Indent(-1);
      method.mBody.Add("}} else {{");
      method.mBody.Indent(1);
      GenerateParseJsonForField(cls, method.mBody, *singleField, "json");
      method.mBody.Indent(-1);
      method.mBody.Add("}}"); // else
    }
  }
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonPlugin::GenerateParseJsonForField(Class &cls, CodeBlock &codeBlock,
                                           const ClassField &field, const std::string &varName) {
  const std::string *variantRawName = nullptr;
  if (field.mField && field.mField->GetAnnotation(Annotations::JsonConvert)) {
    GenerateParseJsonJsonConvert(cls, codeBlock, field, varName);
  } else if (IsVariantTypeField(cls, field, &variantRawName)) {
    GenerateParseJsonVariantType(cls, codeBlock, field, varName, *variantRawName);
  } else if (field.mField && field.mField->mType.mName == St::Variant) {
    GenerateParseJsonVariant(cls, codeBlock, field, varName);
  } else {
    codeBlock.Add("auto res = {}::{}({}, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                  field.mName, varName);
    codeBlock.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
        cls.mStruct->mName, field.mField->mName);
  }
}

void JsonPlugin::GenerateParseJsonVariant(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                          const std::string &varName) {
  auto variantTypeField =
      cls.GetField(Naming().FieldNameInCpp(field.mField->GetAnnotation(Annotations::Variant)
                                               ->GetAttribute(Annotations::Variant_TypeField)
                                               ->mValue.mName));
  // variants with implicit types are not supported
  auto enumName = mProject.mProject.GetEnum(variantTypeField->mType.mName)->mName;
  bool isFirst = true;
  codeBlock.Add("bool res;");
  for (auto &projectStruct: mProject.mProject.mStructs) {
    if (projectStruct.mIsMixin)
      continue;
    // TODO: don't allow variant structs to be inherited from
    auto structVariantAnnotation = projectStruct.GetAnnotation(Annotations::Variant);
    if (!structVariantAnnotation ||
        structVariantAnnotation->GetAttribute(Annotations::Variant_Enum)->mValue.mName != enumName)
      continue;
    auto entryStr = std::format(
        "{}::{}", enumName,
        structVariantAnnotation->GetAttribute(Annotations::Variant_Entry)->mValue.mName);
    if (isFirst) {
      codeBlock.Add("if ({} == {}) {{", variantTypeField->mName, entryStr);
      isFirst = false;
    } else {
      codeBlock.Add("}} else if ({} == {}) {{", variantTypeField->mName, entryStr);
    }
    codeBlock.Indent(1);
    codeBlock.Add("res = {}::{}(*{}(), {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                  Naming().VariantGetterNameInCpp(*field.mField, projectStruct), varName);
    codeBlock.Indent(-1);
  }
  codeBlock.Add("}} else {{");
  codeBlock.Indent(1);
  codeBlock.Add(
      R"R(HOLGEN_WARN("Could not json-parse {}.{} variant field, its type {{}} is unexpected", {});)R",
      cls.mStruct->mName, field.mField->mName, variantTypeField->mName);
  codeBlock.Add("return false;");
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  codeBlock.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} variant field of type {{}}", {});)R",
      cls.mStruct->mName, field.mField->mName, variantTypeField->mName);
}

void JsonPlugin::GenerateParseJsonVariantType(Class &cls, CodeBlock &codeBlock,
                                              const ClassField &field, const std::string &varName,
                                              const std::string &rawFieldName) {
  codeBlock.Add("{}temp;", field.mType.ToString(false));
  codeBlock.Add("auto res = {}::{}(temp, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                varName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                cls.mStruct->mName, rawFieldName);
  codeBlock.Add("{}(temp);", Naming().FieldSetterNameInCpp(rawFieldName));
}

void JsonPlugin::GenerateParseJsonJsonConvert(Class &cls, CodeBlock &codeBlock,
                                              const ClassField &field, const std::string &varName) {
  auto jsonConvert = field.mField->GetAnnotation(Annotations::JsonConvert);
  auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
  auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
  auto jsonConvertElem = jsonConvert->GetAttribute(Annotations::JsonConvert_Elem);
  if (jsonConvertElem) {
    codeBlock.Add("auto res = {}::{}<{}>({}, {}, converter, converter.{});", St::JsonHelper,
                  St::JsonHelper_Parse, Type{mProject, jsonConvertFrom->mValue}.ToString(true),
                  field.mName, varName, jsonConvertUsing->mValue.mName);
    codeBlock.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
        cls.mStruct->mName, field.mField->mName);
  } else {
    Type type(mProject, jsonConvertFrom->mValue);
    codeBlock.Line() << type.ToString(false) << "temp;";
    codeBlock.Add("auto res = {}::{}(temp, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                  varName);
    codeBlock.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
        cls.mStruct->mName, field.mField->mName);
    if (TypeInfo::Get().CppPrimitives.contains(field.mType.mName) ||
        field.mType.mType == PassByType::Pointer)
      codeBlock.Add("{} = converter.{}(temp);", field.mName, jsonConvertUsing->mValue.mName);
    else
      codeBlock.Add("{} = std::move(converter.{}(temp));", field.mName,
                    jsonConvertUsing->mValue.mName);
  }
}

void JsonPlugin::ProcessStruct(Class &cls) {
  if (cls.mStruct->GetAnnotation(Annotations::NoJson))
    return;
  cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
  cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");
  if (cls.mStruct->GetAnnotation(Annotations::DataManager) == nullptr)
    // TODO: currently we iterate over the json obj when deserializing, but this wouldn't work with
    // dependencies. For DataManager we should have something custom so that a single file can
    // define everything too. But for now ParseFiles is good enough.
    GenerateParseJson(cls);
}

void JsonPlugin::ProcessEnum(Class &cls) {
  if (cls.mEnum->GetAnnotation(Annotations::NoJson))
    return;
  cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
  cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");
  auto method = ClassMethod{St::ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  method.mBody.Add("if (json.IsString()) {{");
  method.mBody.Indent(1);
  method.mBody.Add(
      "*this = {}::FromString(std::string_view(json.GetString(), json.GetStringLength()));",
      cls.mName);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else if (json.IsInt64()) {{");
  method.mBody.Indent(1);
  method.mBody.Add("*this = {}(json.GetInt64());", cls.mName);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add("*this = {}{{}};", cls.mName);
  method.mBody.Add(R"R(HOLGEN_WARN("Could not json-parse {} enum: invalid json type");)R",
                   cls.mName);
  method.mBody.Add("return false;");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("return true;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonPlugin::GenerateParseJsonForFunction(Class &cls, CodeBlock &codeBlock,
                                              const ClassMethod &luaFunction) {
  codeBlock.Add("auto res = {}::{}({}, data.value, converter);", St::JsonHelper,
                St::JsonHelper_Parse, Naming().LuaFunctionHandleNameInCpp(*luaFunction.mFunction));
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{}");)R",
                cls.mStruct->mName, luaFunction.mFunction->mName);
}

bool JsonPlugin::IsVariantTypeField(const Class &cls, const ClassField &field,
                                    const std::string **rawName) {
  for (auto &classField: cls.mFields) {
    if (!classField.mField || !classField.mField->GetAnnotation(Annotations::Variant))
      continue;
    auto variantAnnotation = classField.mField->GetAnnotation(Annotations::Variant);
    auto variantFieldAttribute = variantAnnotation->GetAttribute(Annotations::Variant_TypeField);
    if (Naming().FieldNameInCpp(variantFieldAttribute->mValue.mName) == field.mName) {
      if (rawName) {
        *rawName = &variantFieldAttribute->mValue.mName;
      }
      return true;
    }
  }
  return false;
}
} // namespace holgen
