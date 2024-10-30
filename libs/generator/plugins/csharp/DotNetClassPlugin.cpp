#include "DotNetClassPlugin.h"

namespace holgen {
void DotNetClassPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::CSharp))
    return;
  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    Process(cls);
  }
}

void DotNetClassPlugin::Process(Class &cls) const {
  mProject.mCSharpClasses.emplace_back(cls.mName, &cls);
}

bool DotNetClassPlugin::ShouldProcess(const Class &cls) const {
  if (!cls.mStruct ||
      cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp) ||
       cls.mStruct->GetAnnotation(Annotations::DotNetModule) ||
       cls.mStruct->GetAnnotation(Annotations::Interface))
    return false;
  return true;
}
} // namespace holgen
