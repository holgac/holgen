#include "CompositeIdHelper.h"

#include <core/Annotations.h>

namespace holgen {
const ClassField *CompositeIdHelper::GetTypeField(const Class &cls) {
  return GetFieldWithAttribute(cls, Annotations::CompositeIdType_Type, true);
}

const ClassField *CompositeIdHelper::GetIdField(const Class &cls) {
  return GetFieldWithAttribute(cls, Annotations::CompositeIdType_Id, true);
}

const ClassField *CompositeIdHelper::GetVersionField(const Class &cls) {
  return GetFieldWithAttribute(cls, Annotations::CompositeIdType_Version, true);
}

const ClassField *CompositeIdHelper::GetFieldWithAttribute(const Class &cls,
                                                           const std::string &attribute,
                                                           const bool throwOnFailure) {
  for (auto &field: cls.mFields) {
    if (field.mField &&
        field.mField->GetMatchingAnnotation(Annotations::CompositeIdType, attribute))
      return &field;
  }
  THROW_IF(throwOnFailure, "{} ({}) is a composite id type without {}!", cls.mName,
           cls.mStruct->mDefinitionSource, attribute);
  return nullptr;
}
} // namespace holgen
