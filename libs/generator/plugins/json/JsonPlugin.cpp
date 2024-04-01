#include "JsonPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  void JsonPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct)
        ProcessStruct(cls);
      else if (cls.mEnum)
        ProcessEnum(cls);
    }
  }

  void JsonPlugin::GenerateParseJson(Class &cls) {
    auto method = ClassMethod{St::ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back(
        "json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back(
        "converter", Type{St::Converter, PassByType::Reference, Constness::Const});

    method.mBody.Line() << "for(const auto& data: json.GetObject()) {";
    method.mBody.Indent(1);
    method.mBody.Line() << "const auto& name = data.name.GetString();";
    // parseFunc.mBody.Line() << "const auto& value = data.value;";
    // TODO: a helper class for string switching below
    // Pass source string (and optional extra conds), and a map<case, action> and it generates the code below.
    // later it can be smarter (i.e. switch on one of the chars to split as much as possible)
    bool isFirst = true;
    for (const auto &field: cls.mFields) {
      if (!field.mField || field.mField->GetAnnotation(Annotations::NoJson))
        continue;
      if (isFirst) {
        method.mBody.Line() << "if (0 == strcmp(name, \"" << field.mField->mName << "\")) {";
        isFirst = false;
      } else {
        method.mBody.Line() << "} else if (0 == strcmp(name, \"" << field.mField->mName << "\")) {";
      }
      method.mBody.Indent(1); // if name == fieldName
      GenerateParseJsonForField(method, field);
      method.mBody.Indent(-1); // if name == fieldName
    }

    for (const auto &luaMethod: cls.mMethods) {
      if (!luaMethod.mFunction || luaMethod.mFunction->GetAnnotation(Annotations::NoJson) ||
          !luaMethod.mFunction->GetAnnotation(Annotations::LuaFunc))
        continue;
      if (isFirst) {
        method.mBody.Line() << "if (0 == strcmp(name, \"" << luaMethod.mFunction->mName << "\")) {";
        isFirst = false;
      } else {
        method.mBody.Line() << "} else if (0 == strcmp(name, \"" << luaMethod.mFunction->mName << "\")) {";
      }
      method.mBody.Indent(1); // if name == fieldName
      GenerateParseJsonForFunction(method, luaMethod);
      method.mBody.Indent(-1); // if name == fieldName
    }

    if (!isFirst)
      method.mBody.Line() << "}";
    method.mBody.Indent(-1);
    method.mBody.Line() << "}"; // range based for on json.GetObject()
    method.mBody.Line() << "return true;";
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void JsonPlugin::GenerateParseJsonForField(ClassMethod &method, const ClassField &field) {
    if (mProject.GetClass(field.mType.mName) == nullptr) {
      auto jsonConvert = field.mField->GetAnnotation(Annotations::JsonConvert);
      if (jsonConvert != nullptr) {
        auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
        Type type(mProject.mProject, jsonConvertFrom->mValue);
        method.mBody.Line() << type.ToString() << " temp;";
        method.mBody.Add("auto res = {}::{}(temp, data.value, converter);", St::JsonHelper, St::JsonHelper_Parse);
        method.mBody.Line() << "if (!res)";
        method.mBody.Indent(1);
        method.mBody.Line() << "return false;";
        method.mBody.Indent(-1); // if !res
        if (TypeInfo::Get().CppPrimitives.contains(field.mType.mName) || field.mType.mType == PassByType::Pointer)
          method.mBody.Add("{} = converter.{}(temp);", field.mName, jsonConvertUsing->mValue.mName);
        else
          method.mBody.Add("{} = std::move(converter.{}(temp));", field.mName, jsonConvertUsing->mValue.mName);
      } else {
        method.mBody.Add("auto res = {}::{}({}, data.value, converter);", St::JsonHelper, St::JsonHelper_Parse,
                         field.mName);
        method.mBody.Line() << "if (!res)";
        method.mBody.Indent(1);
        method.mBody.Line() << "return false;";
        method.mBody.Indent(-1); // if !res
      }
    } else {
      // TODO: this case is not necessary - JsonHelper::ParseJson handles this
      method.mBody.Add("auto res = {}.{}(data.value, converter);",
                       field.mName, St::ParseJson);

      method.mBody.Line() << "if (!res)";
      method.mBody.Indent(1);
      method.mBody.Line() << "return false;";
      method.mBody.Indent(-1); // if !res
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
      // dependencies. For DataManager we should have something custom so that a single file can define
      // everything too. But for now ParseFiles is good enough.
      GenerateParseJson(cls);
  }

  void JsonPlugin::ProcessEnum(Class &cls) {
    if (cls.mEnum->GetAnnotation(Annotations::NoJson))
      return;
    cls.mHeaderIncludes.AddLibHeader("rapidjson/fwd.h");
    cls.mSourceIncludes.AddLibHeader("rapidjson/document.h");
    cls.mSourceIncludes.AddLocalHeader(St::JsonHelper + ".h");
    auto method = ClassMethod{St::ParseJson, Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{St::Converter, PassByType::Reference, Constness::Const});
    method.mBody.Add("if (json.IsString()) {{");
    method.mBody.Indent(1);
    method.mBody.Add("*this = {}::FromString(std::string_view(json.GetString(), json.GetStringLength()));",
                     cls.mName);
    method.mBody.Indent(-1);
    method.mBody.Add("}} else if (json.IsInt64()) {{");
    method.mBody.Indent(1);
    method.mBody.Add("*this = {}(json.GetInt64());", cls.mName);
    method.mBody.Indent(-1);
    method.mBody.Add("}} else {{");
    method.mBody.Indent(1);
    // TODO: HOLGEN_WARN here
    method.mBody.Add("*this = {}({}::Invalid);", cls.mName, cls.mName);
    method.mBody.Add("return false;");
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
    method.mBody.Add("return true;");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void JsonPlugin::GenerateParseJsonForFunction(ClassMethod &method, const ClassMethod &luaFunction) {
    method.mBody.Add("auto res = {}::{}({}{}, data.value, converter);",
                     St::JsonHelper, St::JsonHelper_Parse,
                     St::LuaFuncPrefix, luaFunction.mFunction->mName);
    method.mBody.Line() << "if (!res)";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1); // if !res
  }
}
