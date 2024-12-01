#include "CompositeIdStructPlugin.h"

#include "core/St.h"
#include "generator/utils/CompositeIdHelper.h"

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
  ProcessFields(cls);
  AddIdGetterMethod(cls);
  AddIsValidMethod(cls);
}

void CompositeIdStructPlugin::ProcessFields(Class &cls) const {
  auto versionField = CompositeIdHelper::GetObjectVersionField(cls);
  if (!versionField->mDefaultValue.has_value())
    cls.GetField(versionField->mName)->mDefaultValue = "0";
}

void CompositeIdStructPlugin::ValidateStruct(const Class &cls) const {
  auto compositeIdAnnotation = cls.mStruct->GetAnnotation(Annotations::CompositeId);
  auto typeAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Type);
  auto entryAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Entry);
  THROW_IF(!typeAttribute || !entryAttribute, "{} ({}) has incomplete composite id annotation!",
           cls.mName, cls.mStruct->mDefinitionSource);
  auto idCls = mProject.GetClass(typeAttribute->mValue.mName);
  THROW_IF(!idCls, "{} ({}) does not have a matching id struct!", cls.mName,
           cls.mStruct->mDefinitionSource);
  auto getterAttribute = idCls->mStruct->GetMatchingAttribute(Annotations::CompositeIdType,
                                                              Annotations::CompositeIdType_Getter);
  THROW_IF(!getterAttribute, "{} ({}) has incomplete composite id annotation!", cls.mName,
           cls.mStruct->mDefinitionSource);
  auto idField = CompositeIdHelper::GetObjectIdField(cls);
  auto versionField = CompositeIdHelper::GetObjectVersionField(cls);
  THROW_IF(!idField || !versionField, "{} ({}) does not have the required id and version fields!",
           cls.mName, cls.mStruct->mDefinitionSource);
  THROW_IF(!TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName),
           "{} ({}) has a signed id field {} ({}) which is not a signed integral type!", cls.mName,
           cls.mStruct->mDefinitionSource, idField->mField->mName,
           idField->mField->mDefinitionSource);
}

void CompositeIdStructPlugin::AddIdGetterMethod(Class &cls) const {
  auto compositeIdAnnotation = cls.mStruct->GetAnnotation(Annotations::CompositeId);
  auto typeAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Type);
  auto entryAttribute = compositeIdAnnotation->GetAttribute(Annotations::CompositeId_Entry);
  auto idCls = mProject.GetClass(typeAttribute->mValue.mName);
  auto methodName =
      idCls->mStruct
          ->GetMatchingAttribute(Annotations::CompositeIdType, Annotations::CompositeIdType_Getter)
          ->mValue.mName;
  if (cls.GetMethod(methodName, Constness::Const))
    return;
  auto method = ClassMethod{methodName, Type{idCls->mName}};
  auto idTypeTypeField = CompositeIdHelper::GetIdTypeField(*idCls);
  auto versionField = CompositeIdHelper::GetObjectVersionField(cls);
  auto idField = CompositeIdHelper::GetObjectIdField(cls);
  method.mBody.Add("return {}({}::{}, {}, {});", idCls->mName, idTypeTypeField->mType.mName,
                   entryAttribute->mValue.mName, idField->mName, versionField->mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void CompositeIdStructPlugin::AddIsValidMethod(Class &cls) const {
  if (cls.GetMethod(St::CompositeId_IsValid, Constness::Const))
    return;

  auto method = ClassMethod{St::CompositeId_IsValid, Type{"bool"}};
  auto idField = CompositeIdHelper::GetObjectIdField(cls);
  method.mBody.Add("return {} >= 0;", idField->mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
