#include "JsonDumpPlugin.h"

#include "core/St.h"

namespace holgen {
void JsonDumpPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }

  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    if (cls.mStruct)
      ProcessStruct(cls);
    else if (cls.mEnum)
      ProcessEnum(cls);
  }
}

void JsonDumpPlugin::ProcessStruct(Class &cls) {
  auto method =
      ClassMethod{St::DumpJson, Type{"rapidjson::Value"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  method.mBody.Add("rapidjson::Value val(rapidjson::kObjectType);");
  for (auto &field: cls.mFields) {
    if (!field.mField || !ShouldProcess(field, false))
      continue;
    GenerateForField(method.mBody, field, field.mField->mName);
  }
  method.mBody.Add("return val;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonDumpPlugin::ProcessEnum(Class &cls) {
  auto method =
      ClassMethod{St::DumpJson, Type{"rapidjson::Value"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  method.mBody.Add("return rapidjson::Value(GetValue());");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void JsonDumpPlugin::GenerateForField(CodeBlock &codeBlock, const ClassField &field,
                                      const std::string &fieldName) {
  codeBlock.Add("val.AddMember(\"{}\", {}::{}({}, doc), doc.GetAllocator());", fieldName,
                St::JsonHelper, St::JsonHelper_Dump, field.mName);
}
} // namespace holgen
