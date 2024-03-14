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

  void JsonPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct)
        EnrichClass(cls, *cls.mStruct);
      else if (cls.mEnum)
        EnrichClass(cls, *cls.mEnum);
    }
  }

  void JsonPlugin::GenerateParseJson(Class &cls) {
    const auto &structDefinition = *mProject.mProject.GetStruct(cls.mName);
    auto &parseFunc = cls.mMethods.emplace_back(ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst);
    parseFunc.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    parseFunc.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});

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
      parseFunc.mBody.Indent(1); // if name == fieldName
      GenerateParseJsonForField(cls, parseFunc, structDefinition, fieldDefinition);
      parseFunc.mBody.Indent(-1); // if name == fieldName
    }

    for (const auto &fd: structDefinition.mFunctions) {
      if (fd.GetAnnotation(Annotations::NoJson))
        continue;
      if (isFirst) {
        parseFunc.mBody.Line() << "if (0 == strcmp(name, \"" << fd.mName << "\")) {";
        isFirst = false;
      } else {
        parseFunc.mBody.Line() << "} else if (0 == strcmp(name, \"" << fd.mName << "\")) {";
      }
      parseFunc.mBody.Indent(1); // if name == fieldName
      GenerateParseJsonForFunction(parseFunc, fd);
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
    if (mProject.mProject.GetStruct(fieldDefinition.mType.mName) == nullptr) {
      auto jsonConvert = fieldDefinition.GetAnnotation(Annotations::JsonConvert);
      if (jsonConvert != nullptr) {
        auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
        Type type(mProject.mProject, jsonConvertFrom->mValue);
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
    auto &parseFunc = cls.mMethods.emplace_back(ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst);
    parseFunc.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    parseFunc.mArguments.emplace_back("converter", Type{ConverterName, PassByType::Reference, Constness::Const});
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

  void JsonPlugin::GenerateParseJsonForFunction(
      ClassMethod &parseFunc, const FunctionDefinition &functionDefinition) {
    parseFunc.mBody.Add("std::string val;");
    parseFunc.mBody.Add("{}::{}(val, data.value, converter);",
                        St::JsonHelper, St::JsonHelper_Parse);
    parseFunc.mBody.Add("mFuncName_{} = std::move(val);", functionDefinition.mName);

  }
}
