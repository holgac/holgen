#include "JsonHelperPlugin.h"
#include "core/St.h"
#include "generator/TypeInfo.h"

namespace holgen {
void JsonHelperPlugin::Run() {
  auto cls = Class{St::JsonHelper, mSettings.mNamespace};
  cls.mHeaderIncludes.AddLibHeader("rapidjson/document.h");
  GenerateBaseParse(cls);
  GenerateParseSingleElem(cls);
  for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
    GenerateParseJsonForSingleElemContainer(cls, container, true);
    GenerateParseJsonForSingleElemContainer(cls, container, false);
  }
  for (const auto &container: TypeInfo::Get().CppLists) {
    GenerateParseJsonForSingleElemContainer(cls, container, true);
    GenerateParseJsonForSingleElemContainer(cls, container, false);
  }
  for (const auto &container: TypeInfo::Get().CppSets) {
    GenerateParseJsonForSingleElemContainer(cls, container, true);
    GenerateParseJsonForSingleElemContainer(cls, container, false);
  }
  for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
    for (int withKeyConverter = 0; withKeyConverter < 2; ++withKeyConverter) {
      for (int withElemConverter = 0; withElemConverter < 2; ++withElemConverter) {
        GenerateParseJsonForKeyedContainer(cls, container, withKeyConverter, withElemConverter);
      }
    }
  }
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}

void JsonHelperPlugin::GenerateParseJsonForSingleElemContainer(Class &cls,
                                                               const std::string &container,
                                                               bool withConverter) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  if (withConverter) {
    method.mName = St::JsonHelper_ParseConvertElem;
    method.mTemplateParameters.emplace_back("typename", "SourceType");
  }
  method.mTemplateParameters.emplace_back("typename", "T");

  bool fixedSize = TypeInfo::Get().CppFixedSizeContainers.contains(container);

  if (fixedSize)
    method.mTemplateParameters.emplace_back("size_t", "C");

  {
    auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("T");
    if (fixedSize)
      out.mType.mTemplateParameters.emplace_back("C");
  }

  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  if (withConverter) {
    method.mArguments.emplace_back("elemConverter",
                                   Type{"ElemConverter", PassByType::Reference, Constness::Const});
    method.mTemplateParameters.emplace_back("typename", "ElemConverter");
  }

  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing {}");)R",
      container);
  if (fixedSize)
    method.mBody.Add("size_t writtenItemCount = 0;");

  method.mBody.Line() << "for (const auto& data: json.GetArray()) {";
  if (fixedSize)
    method.mBody.Add(
        R"R(HOLGEN_WARN_AND_RETURN_IF(writtenItemCount >= C, false, "Received more data than what the container can handle in {}");)R",
        container);
  method.mBody.Indent(1);
  if (withConverter)
    method.mBody.Add("SourceType elem;", St::JsonHelper_Parse);
  else if (!fixedSize)
    method.mBody.Add("T elem;", St::JsonHelper_Parse);
  if (withConverter || !fixedSize)
    method.mBody.Add("auto res = {}(elem, data, converter);", St::JsonHelper_Parse);
  else
    method.mBody.Add("auto res = {}(out[writtenItemCount], data, converter);",
                     St::JsonHelper_Parse);

  method.mBody.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of {}");)R",
                   container);
  std::string elemString = withConverter ? "elemConverter(elem)" : "elem";
  if (TypeInfo::Get().CppSets.contains(container)) {
    method.mBody.Add("out.insert(std::move({}));", elemString);
  } else if (fixedSize) {
    if (withConverter) {
      method.mBody.Add("out[writtenItemCount] = std::move(elem);");
    }
    method.mBody.Add("++writtenItemCount;");
  } else {
    method.mBody.Add("out.push_back(std::move({}));", elemString);
  }
  method.mBody.Indent(-1);
  method.mBody.Line() << "}"; // range based for on json.GetArray()
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateBaseParse(Class &cls) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");
  method.mArguments.emplace_back("out", Type{"T", PassByType::Reference});
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  method.mBody.Add("return out.{}(json, converter);", St::ParseJson);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseSingleElem(Class &cls, const std::string &type,
                                               const std::string &validator,
                                               const std::string &getter) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mIsTemplateSpecialization = true;
  method.mArguments.emplace_back("out", Type{type, PassByType::Reference});
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});
  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.{}(), false, "Found type mismatch in json when parsing {}");)R",
      validator, type);
  method.mBody.Line() << "out = json." << getter << "();";
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonHelperPlugin::GenerateParseSingleElem(Class &cls) {
  GenerateParseSingleElem(cls, "int8_t", "IsInt", "GetInt");
  GenerateParseSingleElem(cls, "int16_t", "IsInt", "GetInt");
  GenerateParseSingleElem(cls, "int32_t", "IsInt", "GetInt");
  GenerateParseSingleElem(cls, "int64_t", "IsInt64", "GetInt64");
  GenerateParseSingleElem(cls, "uint8_t", "IsUint", "GetUint");
  GenerateParseSingleElem(cls, "uint16_t", "IsUint", "GetUint");
  GenerateParseSingleElem(cls, "uint32_t", "IsUint", "GetUint");
  GenerateParseSingleElem(cls, "uint64_t", "IsUint64", "GetUint64");
  GenerateParseSingleElem(cls, "float", "IsNumber", "GetFloat");
  GenerateParseSingleElem(cls, "double", "IsNumber", "GetDouble");
  GenerateParseSingleElem(cls, "bool", "IsBool", "GetBool");
  GenerateParseSingleElem(cls, "std::string", "IsString", "GetString");
}

void JsonHelperPlugin::GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container,
                                                          bool withKeyConverter,
                                                          bool withElemConverter) {
  auto method = ClassMethod{St::JsonHelper_Parse, Type{"bool"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};

  if (withKeyConverter) {
    method.mName = St::JsonHelper_ParseConvertKey;
    method.mTemplateParameters.emplace_back("typename", "KeySourceType");
  }
  if (withElemConverter) {
    method.mName = St::JsonHelper_ParseConvertElem;
    method.mTemplateParameters.emplace_back("typename", "ElemSourceType");
  }


  method.mTemplateParameters.emplace_back("typename", "K");
  method.mTemplateParameters.emplace_back("typename", "V");

  {
    auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("K");
    out.mType.mTemplateParameters.emplace_back("V");
  }
  method.mArguments.emplace_back("json",
                                 Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("converter",
                                 Type{St::Converter, PassByType::Reference, Constness::Const});

  if (withKeyConverter) {
    method.mTemplateParameters.emplace_back("typename", "KeyConverter");
    method.mArguments.emplace_back("keyConverter",
                                   Type{"KeyConverter", PassByType::Reference, Constness::Const});
  }
  if (withElemConverter) {
    method.mTemplateParameters.emplace_back("typename", "ElemConverter");
    method.mArguments.emplace_back("elemConverter",
                                   Type{"ElemConverter", PassByType::Reference, Constness::Const});
  }

  if (withKeyConverter && withElemConverter) {
    method.mName = St::JsonHelper_ParseConvertKeyElem;
  }

  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing {}");)R",
      container);

  method.mBody.Line() << "for (const auto& data: json.GetObject()) {";
  method.mBody.Indent(1);

  if (withKeyConverter) {
    method.mBody.Line() << "KeySourceType keyInJson;";
    method.mBody.Add("auto res = {}(keyInJson, data.name, converter);", St::JsonHelper_Parse);
    method.mBody.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of {}");)R",
                     container);
    method.mBody.Line() << "K key = std::move(keyConverter(keyInJson));";
  } else {
    method.mBody.Line() << "K key;";
    method.mBody.Add("auto res = {}(key, data.name, converter);", St::JsonHelper_Parse);
    method.mBody.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of {}");)R",
                     container);
  }

  method.mBody.Line() << "auto[it, insertRes] = out.try_emplace(key, V());";
  method.mBody.Add(
      R"R(HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {{}} when parsing {}", key);)R",
      container);
  if (withElemConverter) {
    method.mBody.Add("ElemSourceType valueRaw;");
    method.mBody.Add("res = {}(valueRaw, data.value, converter);", St::JsonHelper_Parse);
    method.mBody.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of {}");)R",
                     container);
    method.mBody.Add("it->second = std::move(elemConverter(valueRaw));");
  } else {
    method.mBody.Add("res = {}(it->second, data.value, converter);", St::JsonHelper_Parse);
    method.mBody.Add(R"R(HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of {}");)R",
                     container);
  }
  method.mBody.Indent(-1);
  method.mBody.Line() << "}"; // range based for on json.GetArray()
  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
