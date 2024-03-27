#include "JsonHelperPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  namespace {
    std::string ConverterName = "Converter";
    std::string ParseJson = "ParseJson";
  }

  void JsonHelperPlugin::Run() {
    auto &generatedClass = mProject.mClasses.emplace_back(St::JsonHelper);
    generatedClass.mHeaderIncludes.AddLibHeader("rapidjson/document.h");
    GenerateBaseParse(generatedClass);
    GenerateParseSingleElem(generatedClass);
    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      GenerateParseJsonForSingleElemContainer(generatedClass, container);
    }
    for (const auto &container: TypeInfo::Get().CppSets) {
      GenerateParseJsonForSingleElemContainer(generatedClass, container);
    }
    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      GenerateParseJsonForKeyedContainer(generatedClass, container);
    }
  }

  void JsonHelperPlugin::GenerateParseJsonForSingleElemContainer(Class &cls, const std::string &container) {
    auto &method = cls.mMethods.emplace_back(
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    );
    auto &templateArg = method.mTemplateParameters.emplace_back();
    templateArg.mType = "typename";
    templateArg.mName = "T";
    auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("T");
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

    method.mBody.Line() << "if (!json.IsArray())";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1);

    method.mBody.Line() << "for (const auto& data: json.GetArray()) {";
    method.mBody.Indent(1);
    method.mBody.Add("T elem;", St::JsonHelper_Parse);
    method.mBody.Add("auto res = {}(elem, data, converter);", St::JsonHelper_Parse);
    method.mBody.Line() << "if (!res)";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1); // if !res
    if (TypeInfo::Get().CppSets.contains(container))
      method.mBody.Line() << "out.emplace(std::move(elem));";
    else
      method.mBody.Line() << "out.emplace_back(std::move(elem));";
    method.mBody.Indent(-1);
    method.mBody.Line() << "}"; // range based for on json.GetArray()
    method.mBody.Line() << "return true;";
  }

  void JsonHelperPlugin::GenerateBaseParse(Class &cls) {
    auto &method = cls.mMethods.emplace_back(
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static);
    auto &baseTemplateArg = method.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";
    method.mArguments.emplace_back("out", Type{"T", PassByType::Reference});
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});
    method.mBody.Add("return out.{}(json, converter);", ParseJson);
  }

  void JsonHelperPlugin::GenerateParseSingleElem(Class &cls, const std::string &type, const std::string &validator,
                                                 const std::string &getter) {
    auto &method = cls.mMethods.emplace_back(
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static);
    method.mIsTemplateSpecialization = true;
    method.mArguments.emplace_back("out", Type{type, PassByType::Reference});
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});
    method.mBody.Line() << "if (!json." << validator << "())";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1);
    method.mBody.Line() << "out = json." << getter << "();";
    method.mBody.Line() << "return true;";
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
    GenerateParseSingleElem(cls, "float", "IsFloat", "GetFloat");
    GenerateParseSingleElem(cls, "double", "IsDouble", "GetDouble");
    GenerateParseSingleElem(cls, "bool", "IsBool", "GetBool");
    GenerateParseSingleElem(cls, "std::string", "IsString", "GetString");
  }

  void JsonHelperPlugin::GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container) {
    auto &method = cls.mMethods.emplace_back(
        St::JsonHelper_Parse,
        Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static);
    auto &keyTemplateArg = method.mTemplateParameters.emplace_back();
    keyTemplateArg.mType = "typename";
    keyTemplateArg.mName = "K";
    auto &valueTemplateArg = method.mTemplateParameters.emplace_back();
    valueTemplateArg.mType = "typename";
    valueTemplateArg.mName = "V";

    {
      auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
      out.mType.mTemplateParameters.emplace_back("K");
      out.mType.mTemplateParameters.emplace_back("V");
    }
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

    method.mBody.Line() << "if (!json.IsObject())";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1); // if (!json.IsObject())

    method.mBody.Line() << "for (const auto& data: json.GetObject()) {";
    method.mBody.Indent(1);
    method.mBody.Line() << "K key;";
    method.mBody.Add("{}(key, data.name, converter);", St::JsonHelper_Parse);
    method.mBody.Line() << "auto [it, res] = out.try_emplace(key, V());";
    method.mBody.Line() << "if (!res)";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1); // if !res
    method.mBody.Add("{}(it->second, data.value, converter);", St::JsonHelper_Parse);
    method.mBody.Line() << "if (!res)";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1); // if !res
    method.mBody.Indent(-1);
    method.mBody.Line() << "}"; // range based for on json.GetArray()
    method.mBody.Line() << "return true;";
  }
}