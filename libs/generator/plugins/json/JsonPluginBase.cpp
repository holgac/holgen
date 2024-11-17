#include "JsonPluginBase.h"

#include "core/St.h"

namespace holgen {
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
  if (!method.mFunction || method.mFunction->GetAnnotation(Annotations::NoJson) ||
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
} // namespace holgen
