#include "JsonParsePlugin.h"
#include <vector>
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/utils/StringSwitcher.h"
#include "generator/TypeInfo.h"

namespace holgen {

void JsonParsePlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct)
      ProcessStruct(cls);
    else if (cls.mEnum)
      ProcessEnum(cls);
  }
}

void JsonParsePlugin::GenerateSwitcherLoop(CodeBlock &methodBody, CodeBlock &&codeBlock) {
  methodBody.Add("for (const auto &data: json.GetObject()) {{");
  methodBody.Indent(1);
  methodBody.Add("const auto &name = data.name.GetString();");
  methodBody.Add(std::move(codeBlock));
  methodBody.Indent(-1);
  methodBody.Line() << "}"; // range based for on json.GetObject()
}

void JsonParsePlugin::GenerateParseJsonForLuaFuncTable(Class &cls) {
  auto method = ClassMethod{St::ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  method.mBody.Add("return {}::{}({}, json, converter);", St::JsonHelper, St::JsonHelper_Parse,
                   Naming().FieldNameInCpp(St::LuaTable_TableField));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonParsePlugin::GenerateParseJson(Class &cls) {
  auto method = ClassMethod{St::ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});

  CodeBlock parseJsonFromObjectBlock;
  GenerateParseJsonFromObject(cls, parseJsonFromObjectBlock);
  if (!parseJsonFromObjectBlock.IsEmpty()) {
    method.mBody.Add("if (json.IsObject()) {{");
    method.mBody.Indent(1);
    method.mBody.Add(std::move(parseJsonFromObjectBlock));
    method.mBody.Indent(-1);
    method.mBody.Add("}} else if (json.IsArray()) {{");
    method.mBody.Indent(1);
    GenerateParseJsonFromArray(cls, method.mBody);
    method.mBody.Indent(-1);

    method.mBody.Add("}} else {{");
    method.mBody.Indent(1);
    method.mBody.Add(R"R(HOLGEN_WARN("Unexpected json type when parsing {}.");)R", cls.mName);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  }

  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonParsePlugin::GenerateParseJsonFromArray(Class &cls, CodeBlock &methodBody) {
  methodBody.Add("auto it = json.Begin();");
  for (auto &field: cls.mFields) {
    const std::string *variantRawName = nullptr;
    bool isVariantTypeField = field.IsVariantTypeField(cls, &variantRawName, Naming());
    if (!ShouldProcess(field, isVariantTypeField))
      continue;

    // data manager's container fields are stored in separate files
    if (IsContainerOfDataManager(cls, field))
      continue;

    methodBody.Add("{{");
    methodBody.Indent(1);
    methodBody.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing {}!");)R",
        cls.mName);
    GenerateParseJsonForField(cls, methodBody, field, "(*it)");
    methodBody.Add("++it;");
    methodBody.Indent(-1);
    methodBody.Add("}}");
  }

  for (const auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    methodBody.Add("{{");
    methodBody.Indent(1);
    GenerateParseJsonForFunction(cls, methodBody, method, "(*it)");
    methodBody.Add("++it;");
    methodBody.Indent(-1);
    methodBody.Add("}}");
  }

  methodBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing {}!");)R",
      cls.mName);
}

void JsonParsePlugin::GenerateParseJsonFromObject(Class &cls, CodeBlock &methodBody) {
  StringSwitcher variantSwitcher("name", {});

  CodeBlock stringSwitcherElseCase;
  stringSwitcherElseCase.Add(
      R"R(HOLGEN_WARN("Unexpected entry in json when parsing {}: {{}}", name);)R", cls.mName);
  StringSwitcher switcher("name", std::move(stringSwitcherElseCase));
  for (const auto &field: cls.mFields) {
    const std::string *variantRawName = nullptr;
    bool isVariantTypeField = field.IsVariantTypeField(cls, &variantRawName, Naming());
    if (!ShouldProcess(field, isVariantTypeField))
      continue;
    // data manager's container fields are stored in separate files
    if (IsContainerOfDataManager(cls, field))
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

  for (const auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    switcher.AddCase(method.mFunction->mName, [&](CodeBlock &switchBlock) {
      GenerateParseJsonForFunction(cls, switchBlock, method, "data.value");
    });
  }

  if (!variantSwitcher.IsEmpty()) {
    GenerateSwitcherLoop(methodBody, std::move(variantSwitcher.Generate()));
  }

  if (!switcher.IsEmpty()) {
    GenerateSwitcherLoop(methodBody, std::move(switcher.Generate()));
  }
}

void JsonParsePlugin::GenerateParseJsonForField(Class &cls, CodeBlock &codeBlock,
                                                const ClassField &field,
                                                const std::string &varName) {
  const std::string *variantRawName = nullptr;
  if (field.mField && field.mField->GetAnnotation(Annotations::JsonConvert)) {
    GenerateParseJsonJsonConvert(cls, codeBlock, field, varName);
  } else if (field.IsVariantTypeField(cls, &variantRawName, Naming())) {
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

void JsonParsePlugin::GenerateParseJsonVariant(Class &cls, CodeBlock &codeBlock,
                                               const ClassField &field,
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

void JsonParsePlugin::GenerateParseJsonVariantType(Class &cls, CodeBlock &codeBlock,
                                                   const ClassField &field,
                                                   const std::string &varName,
                                                   const std::string &rawFieldName) {
  codeBlock.Add("{}temp;", field.mType.ToString(false));
  codeBlock.Add("auto res = {}::{}(temp, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                varName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                cls.mStruct->mName, rawFieldName);
  codeBlock.Add("{}(temp);", Naming().FieldSetterNameInCpp(rawFieldName));
}

void JsonParsePlugin::GenerateParseJsonJsonConvertKeyElem(
    Class &cls, CodeBlock &codeBlock, const ClassField &field, const std::string &varName,
    const AnnotationDefinition *convertElemAnnotation,
    const AnnotationDefinition *convertKeyAnnotation) {
  codeBlock.Add("auto res = {}::{}<{}, {}>({}, {}, converter, converter.{}, converter.{});",
                St::JsonHelper, St::JsonHelper_ParseConvertKeyElem,
                Type{mProject, convertKeyAnnotation->mDefinitionSource,
                     convertKeyAnnotation->GetAttribute(Annotations::JsonConvert_From)->mValue}
                    .ToString(true),
                Type{mProject, convertElemAnnotation->mDefinitionSource,
                     convertElemAnnotation->GetAttribute(Annotations::JsonConvert_From)->mValue}
                    .ToString(true),
                field.mName, varName,
                convertKeyAnnotation->GetAttribute(Annotations::JsonConvert_Using)->mValue.mName,
                convertElemAnnotation->GetAttribute(Annotations::JsonConvert_Using)->mValue.mName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                cls.mStruct->mName, field.mField->mName);
}

void JsonParsePlugin::GenerateParseJsonJsonConvertKey(
    Class &cls, CodeBlock &codeBlock, const ClassField &field, const std::string &varName,
    const AnnotationDefinition *convertKeyAnnotation) {
  codeBlock.Add("auto res = {}::{}<{}>({}, {}, converter, converter.{});", St::JsonHelper,
                St::JsonHelper_ParseConvertKey,
                Type{mProject, convertKeyAnnotation->mDefinitionSource,
                     convertKeyAnnotation->GetAttribute(Annotations::JsonConvert_From)->mValue}
                    .ToString(true),
                field.mName, varName,
                convertKeyAnnotation->GetAttribute(Annotations::JsonConvert_Using)->mValue.mName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                cls.mStruct->mName, field.mField->mName);
}

void JsonParsePlugin::GenerateParseJsonJsonConvertElem(
    Class &cls, CodeBlock &codeBlock, const ClassField &field, const std::string &varName,
    const AnnotationDefinition *convertElemAnnotation) {
  codeBlock.Add("auto res = {}::{}<{}>({}, {}, converter, converter.{});", St::JsonHelper,
                St::JsonHelper_ParseConvertElem,
                Type{mProject, convertElemAnnotation->mDefinitionSource,
                     convertElemAnnotation->GetAttribute(Annotations::JsonConvert_From)->mValue}
                    .ToString(true),
                field.mName, varName,
                convertElemAnnotation->GetAttribute(Annotations::JsonConvert_Using)->mValue.mName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                cls.mStruct->mName, field.mField->mName);
}

void JsonParsePlugin::GenerateParseJsonJsonConvertField(Class &cls, CodeBlock &codeBlock,
                                                        const ClassField &field,
                                                        const std::string &varName) {
  auto jsonConvert = field.mField->GetAnnotation(Annotations::JsonConvert);
  auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
  Type type(mProject, jsonConvert->mDefinitionSource,
            jsonConvert->GetAttribute(Annotations::JsonConvert_From)->mValue);
  codeBlock.Line() << type.ToString(false) << "temp;";
  codeBlock.Add("auto res = {}::{}(temp, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                varName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                cls.mStruct->mName, field.mField->mName);
  if (TypeInfo::Get().CppPrimitives.contains(field.mType.mName) ||
      field.mType.mType == PassByType::Pointer)
    codeBlock.Add("{} = converter.{}(temp);", field.mName, jsonConvertUsing->mValue.mName);
  else
    codeBlock.Add("{} = std::move(converter.{}(temp));", field.mName,
                  jsonConvertUsing->mValue.mName);
}

void JsonParsePlugin::GenerateParseJsonJsonConvert(Class &cls, CodeBlock &codeBlock,
                                                   const ClassField &field,
                                                   const std::string &varName) {
  auto convertElemAnnotation = GetConvertElemAnnotation(field.mField);
  auto convertKeyAnnotation = GetConvertKeyAnnotation(field.mField);
  if (convertElemAnnotation && convertKeyAnnotation) {
    GenerateParseJsonJsonConvertKeyElem(cls, codeBlock, field, varName, convertElemAnnotation,
                                        convertKeyAnnotation);
  } else if (convertKeyAnnotation) {
    GenerateParseJsonJsonConvertKey(cls, codeBlock, field, varName, convertKeyAnnotation);
  } else if (convertElemAnnotation) {
    GenerateParseJsonJsonConvertElem(cls, codeBlock, field, varName, convertElemAnnotation);
  } else {
    GenerateParseJsonJsonConvertField(cls, codeBlock, field, varName);
  }
}

void JsonParsePlugin::ProcessStruct(Class &cls) {
  if (cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
    return;
  cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
  cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
  cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");
  if (cls.mStruct->GetAnnotation(Annotations::LuaFuncTable)) {
    GenerateParseJsonForLuaFuncTable(cls);
  } else {
    GenerateParseJson(cls);
  }
}

void JsonParsePlugin::ProcessEnum(Class &cls) {
  if (cls.mEnum->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
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

  if (cls.mEnum->mType == EnumDefinitionType::Bitmap) {
    method.mBody.Add("}} else if (json.IsArray()) {{");
    method.mBody.Indent(1);
    method.mBody.Add("for (auto &data: json.GetArray()) {{");
    method.mBody.Indent(1);
    method.mBody.Add("{} parsedData;", cls.mName);
    method.mBody.Add("parsedData.{}(data, converter);", St::ParseJson);
    method.mBody.Add("Add(parsedData);");
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
    method.mBody.Indent(-1);
  }

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

void JsonParsePlugin::GenerateParseJsonForFunction(Class &cls, CodeBlock &codeBlock,
                                                   const ClassMethod &luaFunction,
                                                   const std::string &varName) {
  codeBlock.Add("auto res = {}::{}({}, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                Naming().LuaFunctionHandleNameInCpp(*luaFunction.mFunction), varName);
  codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{}");)R",
                cls.mStruct->mName, luaFunction.mFunction->mName);
}

const AnnotationDefinition *
    JsonParsePlugin::GetConvertElemAnnotation(const FieldDefinition *field) {
  for (auto &annotation: field->GetAnnotations(Annotations::JsonConvert)) {
    if (annotation.GetAttribute(Annotations::JsonConvert_Elem)) {
      return &annotation;
    }
  }
  return nullptr;
}

const AnnotationDefinition *JsonParsePlugin::GetConvertKeyAnnotation(const FieldDefinition *field) {
  for (auto &annotation: field->GetAnnotations(Annotations::JsonConvert)) {
    if (annotation.GetAttribute(Annotations::JsonConvert_Key)) {
      return &annotation;
    }
  }
  return nullptr;
}
} // namespace holgen
