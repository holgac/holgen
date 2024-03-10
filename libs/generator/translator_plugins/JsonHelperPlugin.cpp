#include "JsonHelperPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  namespace {
    struct RapidJsonTypeUsage {
      std::string mValidator;
      std::string mGetter;
    };

    std::map<std::string, RapidJsonTypeUsage> RapidJsonUsage = {
        {"int8_t",      {"IsInt",    "GetInt"}},
        {"int16_t",     {"IsInt",    "GetInt"}},
        {"int32_t",     {"IsInt",    "GetInt"}},
        {"int64_t",     {"IsInt64",  "GetInt64"}},
        {"uint8_t",     {"IsUint",   "GetUint"}},
        {"uint16_t",    {"IsUint",   "GetUint"}},
        {"uint32_t",    {"IsUint",   "GetUint"}},
        {"uint64_t",    {"IsUint64", "GetUint64"}},
        {"float",       {"IsFloat",  "GetFloat"}},
        {"double",      {"IsDouble", "GetDouble"}},
        {"bool",        {"IsBool",   "GetBool"}},
        {"std::string", {"IsString", "GetString"}},
    };

    std::string ConverterName = "Converter";
    std::string ParseJson = "ParseJson";
  }

  void JsonHelperPlugin::Run() {
    auto &generatedClass = mProject.mClasses.emplace_back(St::JsonHelper);
    generatedClass.mHeaderIncludes.AddLibHeader("rapidjson/document.h");
    auto &baseParse = generatedClass.mMethods.emplace_back(
        St::JsonHelper_Parse, Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static);
    auto &baseTemplateArg = baseParse.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";
    baseParse.mArguments.emplace_back("out", Type{"T", PassByType::Reference});

    baseParse.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    baseParse.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

    baseParse.mBody.Add("return out.{}(json, converter);", ParseJson);

    for (const auto &[type, usage]: RapidJsonUsage) {
      auto &parse = generatedClass.mMethods.emplace_back(
          St::JsonHelper_Parse, Type{"bool"},
          Visibility::Public, Constness::NotConst, Staticness::Static);
      parse.mIsTemplateSpecialization = true;

      parse.mArguments.emplace_back("out", Type{type, PassByType::Reference});
      parse.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
      parse.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

      parse.mBody.Line() << "if (!json." << usage.mValidator << "())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "out = json." << usage.mGetter << "();";
      parse.mBody.Line() << "return true;";
    }

    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      auto &parse = generatedClass.mMethods.emplace_back(
          St::JsonHelper_Parse, Type{"bool"},
          Visibility::Public, Constness::NotConst, Staticness::Static
      );
      auto &templateArg = parse.mTemplateParameters.emplace_back();
      templateArg.mType = "typename";
      templateArg.mName = "T";

      {
        auto &out = parse.mArguments.emplace_back("out", Type{container, PassByType::Reference});
        out.mType.mTemplateParameters.emplace_back("T");
      }
      parse.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
      parse.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

      parse.mBody.Line() << "if (!json.IsArray())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);

      parse.mBody.Line() << "for (const auto& data: json.GetArray()) {";
      parse.mBody.Indent(1);
      parse.mBody.Add("auto res = {}(out.emplace_back(), data, converter);", St::JsonHelper_Parse);
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "}"; // range based for on json.GetArray()
      parse.mBody.Line() << "return true;";

    }

    for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
      auto &parse = generatedClass.mMethods.emplace_back(
          St::JsonHelper_Parse,
          Type{"bool"},
          Visibility::Public, Constness::NotConst, Staticness::Static);
      auto &keyTemplateArg = parse.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = parse.mTemplateParameters.emplace_back();
      valueTemplateArg.mType = "typename";
      valueTemplateArg.mName = "V";

      {
        auto &out = parse.mArguments.emplace_back("out", Type{container, PassByType::Reference});
        out.mType.mTemplateParameters.emplace_back("K");
        out.mType.mTemplateParameters.emplace_back("V");
      }
      parse.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
      parse.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

      parse.mBody.Line() << "if (!json.IsObject())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if (!json.IsObject())

      parse.mBody.Line() << "for (const auto& data: json.GetObject()) {";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "K key;";
      parse.mBody.Add("{}(key, data.name, converter);", St::JsonHelper_Parse);
      parse.mBody.Line() << "auto [it, res] = out.try_emplace(key, V());";
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Add("{}(it->second, data.value, converter);", St::JsonHelper_Parse);
      parse.mBody.Line() << "if (!res)";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1); // if !res
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "}"; // range based for on json.GetArray()
      parse.mBody.Line() << "return true;";
    }
  }
}
