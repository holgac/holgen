#include "ProjectDefinition.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {

  const AnnotationDefinition *FieldDefinition::GetAnnotation(const std::string &name) const {
    for (const auto &annotation: mAnnotations) {
      if (annotation.mName == name)
        return &annotation;
    }
    return nullptr;
  }

  const AnnotationAttributeDefinition *AnnotationDefinition::GetAttribute(const std::string &name) const {
    for (const auto &attribute: mAttributes) {
      if (attribute.mName == name)
        return &attribute;
    }
    return nullptr;
  }

  const AnnotationDefinition *EnumDefinition::GetAnnotation(const std::string &name) const {
    for (const auto &annotation: mAnnotations) {
      if (annotation.mName == name)
        return &annotation;
    }
    return nullptr;
  }

  const AnnotationDefinition *StructDefinition::GetAnnotation(const std::string &name) const {
    for (const auto &annotation: mAnnotations) {
      if (annotation.mName == name)
        return &annotation;
    }
    return nullptr;
  }

  const FieldDefinition *StructDefinition::GetField(const std::string &name) const {
    for (const auto &field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }

  const FieldDefinition *StructDefinition::GetIdField() const {
    for (const auto &field: mFields) {
      if (field.GetAnnotation(Annotations::Id))
        return &field;
    }
    return nullptr;
  }

  const FunctionDefinition *StructDefinition::GetFunction(const std::string &name) const {
    for (const auto &function: mFunctions) {
      if (function.mName == name)
        return &function;
    }
    return nullptr;
  }

  const EnumDefinition *ProjectDefinition::GetEnum(const std::string &name) const {
    for (const auto &enumDefinition: mEnums) {
      if (enumDefinition.mName == name)
        return &enumDefinition;
    }
    return nullptr;
  }

  const StructDefinition *ProjectDefinition::GetStruct(const std::string &name) const {
    for (const auto &structDefinition: mStructs) {
      if (structDefinition.mName == name)
        return &structDefinition;
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

  const EnumEntryDefinition *EnumDefinition::GetEnumEntry(const std::string &name) const {
    for (const auto &entry: mEntries) {
      if (entry.mName == name)
        return &entry;
    }
    return nullptr;
  }
}
