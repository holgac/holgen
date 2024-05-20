#include "ProjectDefinition.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "holgen.h"
#include <optional>

namespace holgen {
  const FieldDefinition *StructDefinition::GetIdField() const {
    for (const auto &field: mFields) {
      if (field.GetAnnotation(Annotations::Id))
        return &field;
    }
    return nullptr;
  }

  bool TypeDefinition::operator==(const TypeDefinition &rhs) const {
    if (mName != rhs.mName)
      return false;
    if (mTemplateParameters.size() != rhs.mTemplateParameters.size())
      return false;
    for (size_t i = 0; i < mTemplateParameters.size(); ++i) {
      if (mTemplateParameters[i] != rhs.mTemplateParameters[i])
        return false;
    }
    return true;
  }

  GEN_GETTER_BY_NAME(FieldDefinition, AnnotationDefinition, GetAnnotation, mAnnotations)

  const AnnotationAttributeDefinition *FieldDefinition::GetMatchingAttribute(
      const std::string &annotationName, const std::string &attributeName,
      std::optional<std::string> attributeValue) const {
    auto annotation = GetAnnotation(annotationName);
    if (!annotation)
      return nullptr;
    for (auto &attribute: annotation->mAttributes) {
      if (attribute.mName == attributeName) {
        if (!attributeValue.has_value() || attribute.mValue.mName == *attributeValue)
          return &attribute;
      }
    }
    return nullptr;
  };

  GEN_GETTER_BY_NAME(AnnotationDefinition, AnnotationAttributeDefinition, GetAttribute, mAttributes);

  GEN_GETTER_BY_NAME(StructDefinition, AnnotationDefinition, GetAnnotation, mAnnotations);

  GEN_GETTER_BY_NAME(StructDefinition, FieldDefinition, GetField, mFields);

  GEN_GETTER_BY_NAME(StructDefinition, FunctionDefinition, GetFunction, mFunctions);

  GEN_GETTER_BY_NAME(ProjectDefinition, EnumDefinition, GetEnum, mEnums);

  GEN_GETTER_BY_NAME(ProjectDefinition, StructDefinition, GetStruct, mStructs);

  GEN_GETTER_BY_NAME(EnumDefinition, EnumEntryDefinition, GetEnumEntry, mEntries);

  GEN_GETTER_BY_NAME(EnumDefinition, AnnotationDefinition, GetAnnotation, mAnnotations);

  GEN_GETTER_BY_NAME(FunctionDefinition, AnnotationDefinition, GetAnnotation, mAnnotations);
}
