#include "JsonPlugin.h"
#include <vector>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  namespace {
    // To represent structs with a single (non-id) field in json,
    // we might want to just use that field instead of a json object.
    // This only works when the field type is primitive or string
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
        if (TypeInfo::Get().CppBasicTypes.contains(singleField->mType.mName))
          return singleField;
        return nullptr;
      }
      // id field is always basic
      return idField;
    }
  }

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

    // TODO: a helper class for string switching below
    // Pass source string (and optional extra conds), and a map<case, action> and it generates the code below.
    // later it can be smarter (i.e. switch on one of the chars to split as much as possible)
    CodeBlock switchBlock;
    bool isFirst = true;
    for (const auto &field: cls.mFields) {
      if (
          !field.mField || field.mField->GetAnnotation(Annotations::NoJson) ||
          field.mField->mType.mName == St::UserData)
        continue;
      if (isFirst) {
        switchBlock.Line() << "if (0 == strcmp(name, \"" << field.mField->mName << "\")) {";
        isFirst = false;
      } else {
        switchBlock.Line() << "} else if (0 == strcmp(name, \"" << field.mField->mName << "\")) {";
      }
      switchBlock.Indent(1); // if name == fieldName
      GenerateParseJsonForField(cls, switchBlock, field, "data.value");
      switchBlock.Indent(-1); // if name == fieldName
    }

    for (const auto &luaMethod: cls.mMethods) {
      if (!luaMethod.mFunction || luaMethod.mFunction->GetAnnotation(Annotations::NoJson) ||
          !luaMethod.mFunction->GetAnnotation(Annotations::LuaFunc))
        continue;
      if (isFirst) {
        switchBlock.Line() << "if (0 == strcmp(name, \"" << luaMethod.mFunction->mName << "\")) {";
        isFirst = false;
      } else {
        switchBlock.Line() << "} else if (0 == strcmp(name, \"" << luaMethod.mFunction->mName << "\")) {";
      }
      switchBlock.Indent(1); // if name == fieldName
      GenerateParseJsonForFunction(cls, switchBlock, luaMethod);
      switchBlock.Indent(-1); // if name == fieldName
    }

    if (!switchBlock.mContents.empty()) {
      auto singleField = GetSingleBasicField(cls);
      if (singleField) {
        method.mBody.Add("if (json.IsObject()) {{");
        method.mBody.Indent(1);
      } else {
        method.mBody.Add(
            R"R(HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing {}");)R",
            cls.mName);
      }

      method.mBody.Add("for(const auto& data: json.GetObject()) {{");
      method.mBody.Indent(1);
      method.mBody.Add("const auto& name = data.name.GetString();");
      method.mBody.Add(std::move(switchBlock));
      method.mBody.Line() << "} else {";
      method.mBody.Indent(1);
      method.mBody.Add(R"R(HOLGEN_WARN("Unexpected entry in json when parsing {}: {{}}", name);)R", cls.mName);
      method.mBody.Indent(-1);
      method.mBody.Line() << "}";
      method.mBody.Indent(-1);
      method.mBody.Line() << "}"; // range based for on json.GetObject()
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

  void JsonPlugin::GenerateParseJsonForField(Class &cls, CodeBlock &codeBlock, const ClassField &field, const std::string &varName) {
    if (mProject.GetClass(field.mType.mName) == nullptr) {
      auto jsonConvert = field.mField->GetAnnotation(Annotations::JsonConvert);
      if (jsonConvert != nullptr) {
        auto jsonConvertFrom = jsonConvert->GetAttribute(Annotations::JsonConvert_From);
        auto jsonConvertUsing = jsonConvert->GetAttribute(Annotations::JsonConvert_Using);
        Type type(mProject.mProject, jsonConvertFrom->mValue);
        codeBlock.Line() << type.ToString() << " temp;";
        codeBlock.Add("auto res = {}::{}(temp, {}, converter);", St::JsonHelper, St::JsonHelper_Parse, varName);
        codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                      cls.mStruct->mName, field.mField->mName);
        if (TypeInfo::Get().CppPrimitives.contains(field.mType.mName) || field.mType.mType == PassByType::Pointer)
          codeBlock.Add("{} = converter.{}(temp);", field.mName, jsonConvertUsing->mValue.mName);
        else
          codeBlock.Add("{} = std::move(converter.{}(temp));", field.mName, jsonConvertUsing->mValue.mName);
      } else {
        codeBlock.Add("auto res = {}::{}({}, {}, converter);", St::JsonHelper, St::JsonHelper_Parse,
                      field.mName, varName);
        codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                      cls.mStruct->mName, field.mField->mName);
      }
    } else {
      // TODO: this case is not necessary - JsonHelper::ParseJson handles this
      codeBlock.Add("auto res = {}.{}(data.value, converter);",
                    field.mName, St::ParseJson);
      codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{} field");)R",
                    cls.mStruct->mName, field.mField->mName);
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
    method.mBody.Add("*this = {}({}::Invalid);", cls.mName, cls.mName);
    method.mBody.Add(R"R(HOLGEN_WARN("Could not json-parse {} enum: invalid json type");)R", cls.mName);
    method.mBody.Add("return false;");
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
    method.mBody.Add("return true;");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void JsonPlugin::GenerateParseJsonForFunction(Class &cls, CodeBlock &codeBlock, const ClassMethod &luaFunction) {
    codeBlock.Add("auto res = {}::{}({}, data.value, converter);",
                  St::JsonHelper, St::JsonHelper_Parse,
                  Naming().LuaFunctionHandleNameInCpp(*luaFunction.mFunction));
    codeBlock.Add(R"R(HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse {}.{}");)R",
                  cls.mStruct->mName, luaFunction.mFunction->mName);
  }
}
