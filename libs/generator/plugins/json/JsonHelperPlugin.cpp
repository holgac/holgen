#include "JsonHelperPlugin.h"
#include "generator/TypeInfo.h"
#include "core/St.h"

namespace holgen {
  void JsonHelperPlugin::Run() {
    auto cls = Class{St::JsonHelper};
    cls.mHeaderIncludes.AddLibHeader("rapidjson/document.h");
    GenerateBaseParse(cls);
    GenerateParseSingleElem(cls);
    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      GenerateParseJsonForSingleElemContainer(cls, container);
    }
    for (const auto &container: TypeInfo::Get().CppSets) {
      GenerateParseJsonForSingleElemContainer(cls, container);
    }
    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      GenerateParseJsonForKeyedContainer(cls, container);
    }
    Validate().NewClass(cls);
    mProject.mClasses.push_back(std::move(cls));
  }

  void JsonHelperPlugin::GenerateParseJsonForSingleElemContainer(Class &cls, const std::string &container) {
    auto method = ClassMethod{
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    };
    method.mTemplateParameters.emplace_back("typename", "T");
    auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
    out.mType.mTemplateParameters.emplace_back("T");
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{St::Converter, PassByType::Reference, Constness::Const});

    // TODO: HOLGEN_WARN
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
      method.mBody.Line() << "out.insert(std::move(elem));";
    else
      method.mBody.Line() << "out.push_back(std::move(elem));";
    method.mBody.Indent(-1);
    method.mBody.Line() << "}"; // range based for on json.GetArray()
    method.mBody.Line() << "return true;";
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void JsonHelperPlugin::GenerateBaseParse(Class &cls) {
    auto method = ClassMethod{
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mTemplateParameters.emplace_back("typename", "T");
    method.mArguments.emplace_back("out", Type{"T", PassByType::Reference});
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{St::Converter, PassByType::Reference, Constness::Const});
    method.mBody.Add("return out.{}(json, converter);", St::ParseJson);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void JsonHelperPlugin::GenerateParseSingleElem(Class &cls, const std::string &type, const std::string &validator,
                                                 const std::string &getter) {
    auto method = ClassMethod{
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mIsTemplateSpecialization = true;
    method.mArguments.emplace_back("out", Type{type, PassByType::Reference});
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{St::Converter, PassByType::Reference, Constness::Const});
    method.mBody.Line() << "if (!json." << validator << "())";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1);
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
    GenerateParseSingleElem(cls, "float", "IsFloat", "GetFloat");
    GenerateParseSingleElem(cls, "double", "IsDouble", "GetDouble");
    GenerateParseSingleElem(cls, "bool", "IsBool", "GetBool");
    GenerateParseSingleElem(cls, "std::string", "IsString", "GetString");
  }

  void JsonHelperPlugin::GenerateParseJsonForKeyedContainer(Class &cls, const std::string &container) {
    auto method = ClassMethod{
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mTemplateParameters.emplace_back("typename", "K");
    method.mTemplateParameters.emplace_back("typename", "V");

    {
      auto &out = method.mArguments.emplace_back("out", Type{container, PassByType::Reference});
      out.mType.mTemplateParameters.emplace_back("K");
      out.mType.mTemplateParameters.emplace_back("V");
    }
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{St::Converter, PassByType::Reference, Constness::Const});

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
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}