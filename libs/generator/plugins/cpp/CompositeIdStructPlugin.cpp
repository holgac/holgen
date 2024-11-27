#include "CompositeIdStructPlugin.h"

#include <generator/utils/CompositeIdHelper.h>

namespace holgen {
void CompositeIdStructPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    Process(cls);
  }
}

bool CompositeIdStructPlugin::ShouldProcess(const Class &cls) const {
  if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::CompositeId))
    return false;
  return true;
}

void CompositeIdStructPlugin::Process(Class &cls) const {
  ValidateStruct(cls);
  AddIdGetterMethod(cls);
}

void CompositeIdStructPlugin::ValidateStruct(const Class &cls) const {
  auto compositeIdAnnotation = cls.mStruct->GetAnnotation(Annotations::CompositeId);
  auto typeAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Type);
  auto entryAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Entry);
  THROW_IF(!typeAttribute || !entryAttribute, "{} ({}) has incomplete  composite id annotation!",
           cls.mName, cls.mStruct->mDefinitionSource);
  auto idType = mProject.GetClass(typeAttribute->mValue.mName);
  auto getterAttribute = idType->mStruct->GetMatchingAttribute(Annotations::CompositeIdType,
                                                               Annotations::CompositeIdType_Getter);
  THROW_IF(!getterAttribute, "{} ({}) has incomplete composite id annotation!", cls.mName,
           cls.mStruct->mDefinitionSource);
}

void CompositeIdStructPlugin::AddIdGetterMethod(Class &cls) const {
  auto compositeIdAnnotation = cls.mStruct->GetAnnotation(Annotations::CompositeId);
  auto typeAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Type);
  auto entryAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Entry);
  auto idType = mProject.GetClass(typeAttribute->mValue.mName);
  auto method = ClassMethod{
      idType->mStruct
          ->GetMatchingAttribute(Annotations::CompositeIdType, Annotations::CompositeIdType_Getter)
          ->mValue.mName,
      Type{idType->mName}};
  auto idTypeTypeField = CompositeIdHelper::GetIdTypeField(*idType);
  auto versionField = CompositeIdHelper::GetObjectVersionField(cls);
  auto idField = CompositeIdHelper::GetObjectIdField(cls);
  method.mBody.Add("return {}({}::{}, {}, {});", idType->mName, idTypeTypeField->mType.mName,
                   entryAttribute->mValue.mName, idField->mName, versionField->mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
