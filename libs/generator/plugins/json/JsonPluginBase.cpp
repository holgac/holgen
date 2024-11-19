#include "JsonPluginBase.h"

#include "core/St.h"

namespace holgen {
JsonPluginBase::JsonPluginBase(TranslatedProject &translatedProject,
                               const TranslatorSettings &translatorSettings) :
    TranslatorPlugin(translatedProject, translatorSettings) {
  if (mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    mLuaStateArgument = ", luaState";
  }
}

bool JsonPluginBase::ShouldProcess(const ClassField &field, bool isVariantType) const {
  if (isVariantType)
    return true;
  if (!field.mField)
    return false;
  if (field.mField->mType.mName == St::UserData || field.mType.mType == PassByType::Pointer)
    return false;
  if (field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
    return false;
  auto cls = mProject.GetClass(field.mField->mName);
  if (cls && !ShouldProcess(*cls))
    return false;
  return true;
}

bool JsonPluginBase::ShouldProcess(const ClassMethod &method) const {
  if (!method.mFunction ||
      method.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Json) ||
      !method.mFunction->GetAnnotation(Annotations::LuaFunc))
    return false;
  return true;
}

bool JsonPluginBase::ShouldProcess(const Class &cls) const {
  if (cls.mStruct && cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
    return false;
  if (cls.mEnum && cls.mEnum->GetMatchingAttribute(Annotations::No, Annotations::No_Json))
    return false;
  return true;
}

bool JsonPluginBase::IsContainerOfDataManager(const Class &cls, const ClassField &field) const {
  return cls.mStruct && cls.mStruct->GetAnnotation(Annotations::DataManager) && field.mField &&
      field.mField->GetAnnotation(Annotations::Container);
}

void JsonPluginBase::GenerateOnDataLoadCalls(const Class &cls, CodeBlock &codeBlock) {
  for (auto &method: cls.mMethods) {
    if (!method.mFunction ||
        !method.mFunction->GetMatchingAnnotation(Annotations::Func, Annotations::Func_OnDataLoad))
      continue;
    DefinitionSource dummySource{"Invalid"};
    for (auto &arg: method.mArguments) {
      THROW_IF(!arg.mDefaultValue.has_value(),
               "{} ({}) has an onDataLoad function {} ({}) that takes a parameter without a "
               "default value!",
               cls.mName, cls.mStruct ? cls.mStruct->mDefinitionSource : dummySource, method.mName,
               method.mFunction->mDefinitionSource);
    }
    codeBlock.Add("{}();", method.mName);
  }
}
} // namespace holgen
