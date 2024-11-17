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
    if (!field.mField || !ShouldProcess(field))
      continue;
    GenerateForField(cls, method.mBody, field, field.mField->mName);
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

void JsonDumpPlugin::GenerateForField(Class &cls, CodeBlock &codeBlock, const ClassField &field,
                                      const std::string &fieldName) {
  (void)cls;
  // TODO: remove this code completely - currently it's used to gate the implemented types
  if (TypeInfo::Get().CppPrimitives.contains(field.mType.mName)) {
  } else if (field.mType.mName == "std::string") {
  } else if (mProject.GetClass(field.mType.mName)) {
  } else {
    return;
  }
  codeBlock.Add("val.AddMember(\"{}\", {}::{}({}, doc), doc.GetAllocator());", fieldName,
                St::JsonHelper, St::JsonHelper_Dump, field.mName);
}

bool JsonDumpPlugin::ShouldProcess(const ClassField &field) {
  if (field.mField) {
    if (field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
      return false;
    auto cls = mProject.GetClass(field.mField->mName);
    if (cls && !ShouldProcess(*cls))
      return false;
  }
  return true;
}

bool JsonDumpPlugin::ShouldProcess(const Class &cls) {
  if (cls.mStruct && cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
    return false;
  if (cls.mEnum && cls.mEnum->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
    return false;
  return true;
}
} // namespace holgen
