#include "CompositeIdHelper.h"

#include "core/Annotations.h"
#include "generator/TranslatedProject.h"

namespace holgen {
const ClassField *CompositeIdHelper::GetObjectTypeField(const Class &cls) {
  return GetObjectFieldWithAttribute(cls, Annotations::CompositeId_Type, true);
}

const ClassField *CompositeIdHelper::GetObjectIdField(const Class &cls, const bool throwOnFailure) {
  return GetObjectFieldWithAttribute(cls, Annotations::CompositeId_Id, throwOnFailure);
}

const ClassField *CompositeIdHelper::GetObjectVersionField(const Class &cls) {
  return GetObjectFieldWithAttribute(cls, Annotations::CompositeId_Version, true);
}

const ClassField *CompositeIdHelper::GetIdTypeField(const Class &cls) {
  return GetIdFieldWithAttribute(cls, Annotations::CompositeIdType_Type, true);
}

const ClassField *CompositeIdHelper::GetIdIdField(const Class &cls) {
  return GetIdFieldWithAttribute(cls, Annotations::CompositeIdType_Id, true);
}

const ClassField *CompositeIdHelper::GetIdVersionField(const Class &cls) {
  return GetIdFieldWithAttribute(cls, Annotations::CompositeIdType_Version, true);
}

const Class *CompositeIdHelper::GetCompositeIdType(const TranslatedProject &project,
                                                   const Class &cls) {
  if (!cls.mStruct)
    return nullptr;
  auto attribute = cls.mStruct->GetMatchingAttribute(Annotations::CompositeId, Annotations::CompositeId_Type);
  if (!attribute)
    return nullptr;
  return project.GetClass(attribute->mValue.mName);
}

const ClassField *CompositeIdHelper::GetIdFieldWithAttribute(const Class &cls,
                                                             const std::string &attribute,
                                                             const bool throwOnFailure) {
  return GetFieldWithAttribute(cls, Annotations::CompositeIdType, attribute, throwOnFailure);
}

const ClassField *CompositeIdHelper::GetObjectFieldWithAttribute(const Class &cls,
                                                                 const std::string &attribute,
                                                                 const bool throwOnFailure) {
  return GetFieldWithAttribute(cls, Annotations::CompositeId, attribute, throwOnFailure);
}

const ClassField *CompositeIdHelper::GetFieldWithAttribute(const Class &cls,
                                                           const std::string &annotation,
                                                           const std::string &attribute,
                                                           const bool throwOnFailure) {
  for (auto &field: cls.mFields) {
    if (field.mField && field.mField->GetMatchingAnnotation(annotation, attribute))
      return &field;
  }
  THROW_IF(throwOnFailure, "{} ({}) is a composite id type without {}!", cls.mName,
           cls.mStruct->mDefinitionSource, attribute);
  return nullptr;
}
} // namespace holgen
