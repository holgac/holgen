#include "ProjectDefinition.h"
#include <optional>
#include "core/Annotations.h"
#include "core/St.h"
#include "core/Exception.h"
#include "holgen.h"


#define GEN_GET_MATCHING_ATTRIBUTE(clsName) \
  const AnnotationAttributeDefinition *clsName::GetMatchingAttribute( \
      const std::string &annotationName, const std::string &attributeName, \
      std::optional<std::string> attributeValue) const { \
    auto annotation = GetAnnotation(annotationName); \
    if (!annotation) \
      return nullptr; \
    for (auto &attribute: annotation->mAttributes) { \
      if (attribute.mName == attributeName) { \
        if (!attributeValue.has_value() || attribute.mValue.mName == *attributeValue) \
          return &attribute; \
      } \
    } \
    return nullptr; \
  };

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

GEN_GET_MATCHING_ATTRIBUTE(FieldDefinition);
GEN_GET_MATCHING_ATTRIBUTE(FunctionDefinition);
GEN_GET_MATCHING_ATTRIBUTE(StructDefinition);
GEN_GET_MATCHING_ATTRIBUTE(EnumEntryDefinition);

GEN_GETTER_BY_NAME(AnnotationDefinition, AnnotationAttributeDefinition, GetAttribute, mAttributes);

GEN_GETTER_BY_NAME(StructDefinition, AnnotationDefinition, GetAnnotation, mAnnotations);

GEN_GETTER_BY_NAME(StructDefinition, FieldDefinition, GetField, mFields);

GEN_GETTER_BY_NAME(StructDefinition, FunctionDefinition, GetFunction, mFunctions);

GEN_GETTER_BY_NAME(ProjectDefinition, EnumDefinition, GetEnum, mEnums);

GEN_GETTER_BY_NAME(ProjectDefinition, StructDefinition, GetStruct, mStructs);

GEN_GETTER_BY_NAME(EnumDefinition, EnumEntryDefinition, GetEnumEntry, mEntries);

GEN_GETTER_BY_NAME(EnumDefinition, AnnotationDefinition, GetAnnotation, mAnnotations)

const EnumEntryDefinition *EnumDefinition::GetDefaultEntry() const {
  const EnumEntryDefinition *defaultEntry = nullptr;
  for (auto &entry: mEntries) {
    if (entry.GetMatchingAttribute(Annotations::Enum, Annotations::Enum_Default)) {
      THROW_IF(defaultEntry, "Enum {} has multiple default entries: {} ({}) and {} ({})", mName,
               defaultEntry->mName, defaultEntry->mDefinitionSource, entry.mName,
               entry.mDefinitionSource)
      defaultEntry = &entry;
    }
  }
  return defaultEntry;
};

GEN_GETTER_BY_NAME(EnumEntryDefinition, AnnotationDefinition, GetAnnotation, mAnnotations);

GEN_GETTER_BY_NAME(FunctionDefinition, AnnotationDefinition, GetAnnotation, mAnnotations);
} // namespace holgen
