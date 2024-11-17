#include "JsonDumpPlugin.h"

#include "core/St.h"

namespace holgen {
void JsonDumpPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Json)) {
    return;
  }

  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct)
      ProcessStruct(cls);
    else if (cls.mEnum)
      ProcessEnum(cls);
  }
}

void JsonDumpPlugin::ProcessStruct(Class &cls) {
  (void)cls;
}

void JsonDumpPlugin::ProcessEnum(Class &cls) {
  auto method =
      ClassMethod{St::DumpJson, Type{"rapidjson::Value"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("doc", Type{"rapidjson::Document", PassByType::Reference});
  method.mBody.Add("return rapidjson::Value(GetValue());");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
