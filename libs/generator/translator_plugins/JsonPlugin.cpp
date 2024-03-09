#include "JsonPlugin.h"
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

  void JsonPlugin::EnrichClasses() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct)
        EnrichClass(cls, *cls.mStruct);
      else if (cls.mEnum)
        EnrichClass(cls, *cls.mEnum);
    }
  }

  void JsonPlugin::GenerateParseJson(Class &cls) {
    const auto &structDefinition = *mProject.mProject.GetStruct(cls.mName);
    auto &parseFunc = cls.mMethods.emplace_back();
    parseFunc.mName = ParseJson;
    parseFunc.mConstness = Constness::NotConst;
    parseFunc.mReturnType.mName = "bool";
    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = "rapidjson::Value";
      arg.mType.mType = PassByType::Reference;
      arg.mType.mConstness = Constness::Const;
      arg.mName = "json";
    }

    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = ConverterName;
      arg.mType.mType = PassByType::Reference;
      arg.mType.mConstness = Constness::Const;
      arg.mName = "converter";
    }
    parseFunc.mBody.Line() << "for(const auto& data: json.GetObject()) {";
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Line() << "const auto& name = data.name.GetString();";
    // parseFunc.mBody.Line() << "const auto& value = data.value;";
    bool isFirst = true;
    for (const auto &fieldDefinition: structDefinition.mFields) {
      if (fieldDefinition.GetAnnotation(Annotations::NoJson))
        continue;
      if (isFirst) {
        parseFunc.mBody.Line() << "if (0 == strcmp(name, \"" << fieldDefinition.mName << "\")) {";
        isFirst = false;
      } else {
        parseFunc.mBody.Line() << "} else if (0 == strcmp(name, \"" << fieldDefinition.mName << "\")) {";
      }
      GenerateParseJsonForField(cls, parseFunc, structDefinition, fieldDefinition);
      parseFunc.mBody.Indent(-1); // if name == fieldName
    }

    if (!structDefinition.mFields.empty())
      parseFunc.mBody.Line() << "}";
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Line() << "}"; // range based for on json.GetObject()
    parseFunc.mBody.Line() << "return true;";
  }

  void JsonPlugin::GenerateParseJsonForField(
      Class &cls __attribute__((unused)),
      ClassMethod &parseFunc,
      const StructDefinition &structDefinition __attribute__((unused)),
      const FieldDefinition &fieldDefinition
  ) {
    parseFunc.mBody.Indent(1);

    if (mProject.mProject.GetStruct(fieldDefinition.mType.mName) == nullptr) {
      auto jsonConvert = fieldDefinition.GetAnnotation(Annotations::JsonConvert);
      if (jsonConvert != nullptr) {
        auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
        Type type;
        TypeInfo::Get().ConvertToType(type, jsonConvertFrom->mValue);
        parseFunc.mBody.Line() << type.ToString() << " temp;";
        parseFunc.mBody.Add("auto res = {}::{}(temp, data.value, converter);", St::JsonHelper, St::JsonHelper_Parse);
        parseFunc.mBody.Line() << "if (!res)";
        parseFunc.mBody.Indent(1);
        parseFunc.mBody.Line() << "return false;";
        parseFunc.mBody.Indent(-1); // if !res

        auto field = cls.GetField(St::GetFieldNameInCpp(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref"));
        if (TypeInfo::Get().CppPrimitives.contains(field->mType.mName))
          parseFunc.mBody.Add("{} = converter.{}(temp);", field->mName, jsonConvertUsing->mValue.mName);
        else
          parseFunc.mBody.Add("{} = std::move(converter.{}(temp));", field->mName, jsonConvertUsing->mValue.mName);
      } else {
        parseFunc.mBody.Add("auto res = {}::{}({}, data.value, converter);", St::JsonHelper, St::JsonHelper_Parse,
                            St::GetFieldNameInCpp(fieldDefinition.mName));
        parseFunc.mBody.Line() << "if (!res)";
        parseFunc.mBody.Indent(1);
        parseFunc.mBody.Line() << "return false;";
        parseFunc.mBody.Indent(-1); // if !res
      }
    } else {
      parseFunc.mBody.Add("auto res = {}.{}(data.value, converter);",
                          St::GetFieldNameInCpp(fieldDefinition.mName), ParseJson);

      parseFunc.mBody.Line() << "if (!res)";
      parseFunc.mBody.Indent(1);
      parseFunc.mBody.Line() << "return false;";
      parseFunc.mBody.Indent(-1); // if !res
    }
  }

  void JsonPlugin::GenerateHelpers() {
    GenerateJsonHelper(mProject.mClasses.emplace_back());
    GenerateConverter(mProject.mClasses.emplace_back());
  }

  // TODO: move to GeneratorJsonHelpers class
  void JsonPlugin::GenerateJsonHelper(Class &generatedClass) {
    generatedClass.mName = St::JsonHelper;
    generatedClass.mHeaderIncludes.AddLibHeader("rapidjson/document.h");
    auto &baseParse = generatedClass.mMethods.emplace_back();
    baseParse.mName = St::JsonHelper_Parse;
    baseParse.mConstness = Constness::NotConst;
    baseParse.mStaticness = Staticness::Static;
    baseParse.mReturnType.mName = "bool";
    auto &baseTemplateArg = baseParse.mTemplateParameters.emplace_back();
    baseTemplateArg.mType = "typename";
    baseTemplateArg.mName = "T";

    {
      auto &out = baseParse.mArguments.emplace_back();
      out.mName = "out";
      out.mType.mName = "T";
      out.mType.mType = PassByType::Reference;
    }

    {
      auto &json = baseParse.mArguments.emplace_back();
      json.mName = "json";
      json.mType.mName = "rapidjson::Value";
      json.mType.mConstness = Constness::Const;
      json.mType.mType = PassByType::Reference;
    }

    {
      auto &converter = baseParse.mArguments.emplace_back();
      converter.mName = "converter";
      converter.mType.mName = ConverterName;
      converter.mType.mConstness = Constness::Const;
      converter.mType.mType = PassByType::Reference;
    }

    baseParse.mBody.Add("return out.{}(json, converter);", ParseJson);

    for (const auto &[type, usage]: RapidJsonUsage) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = St::JsonHelper_Parse;
      parse.mConstness = Constness::NotConst;
      parse.mStaticness = Staticness::Static;
      parse.mIsTemplateSpecialization = true;
      parse.mReturnType.mName = "bool";

      {
        auto &out = parse.mArguments.emplace_back();
        out.mName = "out";
        out.mType.mName = type;
        out.mType.mType = PassByType::Reference;
      }

      {
        auto &json = parse.mArguments.emplace_back();
        json.mName = "json";
        json.mType.mName = "rapidjson::Value";
        json.mType.mConstness = Constness::Const;
        json.mType.mType = PassByType::Reference;
      }

      {
        auto &converter = parse.mArguments.emplace_back();
        converter.mName = "converter";
        converter.mType.mName = ConverterName;
        converter.mType.mConstness = Constness::Const;
        converter.mType.mType = PassByType::Reference;
      }

      parse.mBody.Line() << "if (!json." << usage.mValidator << "())";
      parse.mBody.Indent(1);
      parse.mBody.Line() << "return false;";
      parse.mBody.Indent(-1);
      parse.mBody.Line() << "out = json." << usage.mGetter << "();";
      parse.mBody.Line() << "return true;";
    }

    for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = St::JsonHelper_Parse;
      parse.mConstness = Constness::NotConst;
      parse.mStaticness = Staticness::Static;
      auto &templateArg = parse.mTemplateParameters.emplace_back();
      templateArg.mType = "typename";
      templateArg.mName = "T";
      parse.mReturnType.mName = "bool";

      {
        auto &out = parse.mArguments.emplace_back();
        out.mName = "out";
        out.mType.mName = container;
        auto &outTemplate = out.mType.mTemplateParameters.emplace_back();
        outTemplate.mName = "T";
        out.mType.mType = PassByType::Reference;
      }

      {
        auto &json = parse.mArguments.emplace_back();
        json.mName = "json";
        json.mType.mName = "rapidjson::Value";
        json.mType.mConstness = Constness::Const;
        json.mType.mType = PassByType::Reference;
      }

      {
        auto &converter = parse.mArguments.emplace_back();
        converter.mName = "converter";
        converter.mType.mName = ConverterName;
        converter.mType.mConstness = Constness::Const;
        converter.mType.mType = PassByType::Reference;
      }

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
      auto &parse = generatedClass.mMethods.emplace_back();
      parse.mName = St::JsonHelper_Parse;
      parse.mConstness = Constness::NotConst;
      parse.mStaticness = Staticness::Static;
      auto &keyTemplateArg = parse.mTemplateParameters.emplace_back();
      keyTemplateArg.mType = "typename";
      keyTemplateArg.mName = "K";
      auto &valueTemplateArg = parse.mTemplateParameters.emplace_back();
      valueTemplateArg.mType = "typename";
      valueTemplateArg.mName = "V";
      parse.mReturnType.mName = "bool";

      {
        auto &out = parse.mArguments.emplace_back();
        out.mName = "out";
        out.mType.mName = container;
        auto &keyTemplate = out.mType.mTemplateParameters.emplace_back();
        keyTemplate.mName = "K";
        auto &valueTemplate = out.mType.mTemplateParameters.emplace_back();
        valueTemplate.mName = "V";
        out.mType.mType = PassByType::Reference;
      }

      {
        auto &json = parse.mArguments.emplace_back();
        json.mName = "json";
        json.mType.mName = "rapidjson::Value";
        json.mType.mConstness = Constness::Const;
        json.mType.mType = PassByType::Reference;
      }

      {
        auto &converter = parse.mArguments.emplace_back();
        converter.mName = "converter";
        converter.mType.mName = ConverterName;
        converter.mType.mConstness = Constness::Const;
        converter.mType.mType = PassByType::Reference;
      }

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

  void JsonPlugin::GenerateConverter(Class &cls) {
    cls.mName = "Converter";
    std::set<std::string> processedConverters;
    for (const auto &structDefinition: mProject.mProject.mStructs) {
      for (const auto &fieldDefinition: structDefinition.mFields) {
        auto jsonConvert = fieldDefinition.GetAnnotation(Annotations::JsonConvert);
        if (jsonConvert == nullptr)
          continue;
        auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
        if (processedConverters.contains(jsonConvertUsing->mValue.mName))
          continue;
        processedConverters.insert(jsonConvertUsing->mValue.mName);
        auto &func = cls.mFields.emplace_back();
        func.mVisibility = Visibility::Public;
        func.mName = jsonConvertUsing->mValue.mName;
        func.mType.mName = "std::function";

        auto &convertFromArg = func.mType.mFunctionalTemplateParameters.emplace_back();

        auto fieldNameInCpp = St::GetFieldNameInCpp(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref");
        auto referencedClass = mProject.GetClass(structDefinition.mName);

        convertFromArg = referencedClass->GetField(fieldNameInCpp)->mType;
        // TypeInfo::Get().ConvertToType(convertFromArg, fieldDefinition.mType);

        auto &retVal = func.mType.mFunctionalTemplateParameters.emplace_back();
        TypeInfo::Get().ConvertToType(retVal, jsonConvertFrom->mValue);
        if (!TypeInfo::Get().CppPrimitives.contains(retVal.mName)) {
          retVal.mConstness = Constness::Const;
          retVal.mType = PassByType::Reference;
        }

      }
    }

  }

  void JsonPlugin::EnrichClass(Class &cls, const StructDefinition &structDefinition) {
    if (structDefinition.GetAnnotation(Annotations::NoJson))
      return;
    cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
    cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
    cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");
    if (structDefinition.GetAnnotation(Annotations::DataManager) == nullptr)
      // TODO: currently we iterate over the json obj when deserializing, but this wouldn't work with
      // dependencies. For DataManager we should have something custom so that a single file can define
      // everything too. But for now ParseFiles is good enough.
      GenerateParseJson(cls);
  }


  void JsonPlugin::EnrichClass(Class &cls, const EnumDefinition &enumDefinition) {
    if (enumDefinition.GetAnnotation(Annotations::NoJson))
      return;
    cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
    cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
    cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");
    auto &parseFunc = cls.mMethods.emplace_back();
    parseFunc.mName = ParseJson;
    parseFunc.mConstness = Constness::NotConst;
    parseFunc.mReturnType.mName = "bool";
    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = "rapidjson::Value";
      arg.mType.mType = PassByType::Reference;
      arg.mType.mConstness = Constness::Const;
      arg.mName = "json";
    }

    {
      auto &arg = parseFunc.mArguments.emplace_back();
      arg.mType.mName = ConverterName;
      arg.mType.mType = PassByType::Reference;
      arg.mType.mConstness = Constness::Const;
      arg.mName = "converter";
    }
    parseFunc.mBody.Add("if (json.IsString()) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("*this = {}::FromString(std::string_view(json.GetString(), json.GetStringLength()));",
                        cls.mName);
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}} else if (json.IsInt64()) {{");
    parseFunc.mBody.Indent(1);
    parseFunc.mBody.Add("*this = {}(json.GetInt64());", cls.mName);
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}} else {{");
    parseFunc.mBody.Indent(1);
    // TODO: logging?
    parseFunc.mBody.Add("*this = {}({}::Invalid);", cls.mName, cls.mName);
    parseFunc.mBody.Add("return false;");
    parseFunc.mBody.Indent(-1);
    parseFunc.mBody.Add("}}");
    parseFunc.mBody.Add("return true;");
  }
}
