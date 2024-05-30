#pragma once

#include "TranslatedProject.h"
#include "NamingConvention.h"

namespace holgen {
class Validator {
public:
  explicit Validator(TranslatedProject &project, const NamingConvention &naming);
  void NewClass(const Class &cls) const;
  void IdField(const Class &cls, const ClassField &field) const;
  void NewField(const Class &cls, const ClassField &field) const;
  void RefField(const Class &cls, const FieldDefinition &fieldDefinition) const;
  void NewMethod(const Class &cls, const ClassMethod &method) const;
  void Enum(const Class &cls) const;
  void NewEnumEntry(const Class &cls, const ClassEnum &nestedEnum, const ClassEnumEntry &entry) const;
  void ContainerAnnotation(
      const Class &cls, const ClassField &field, const AnnotationDefinition &annotationDefinition) const;
  void ManagedAnnotation(const Class &cls, const AnnotationDefinition &annotationDefinition) const;
  void IndexAnnotation(
      const Class &cls, const ClassField &field, const AnnotationDefinition &annotationDefinition) const;
  void JsonConverters() const;
private:
  void ValidateType(const Type &type, const Class &cls, bool acceptVoid, const ClassMethod *method,
                    const std::string &source) const;
  void ValidateAttributeCount(const AnnotationDefinition &annotation, const std::string &attributeName,
                              const std::string &source, size_t minCount = 1, size_t maxCount = 1) const;
  void EnforceUniqueAnnotation(const Class &cls, const ClassField &field, const std::string &annotationName) const;
  void EnforceUniqueAnnotation(const Class &cls, const std::string &annotationName) const;
  TranslatedProject &mProject;
  const NamingConvention &mNaming;
  void ValidateMixins(const StructDefinition &structDefinition) const;
  void ValidateMixins(const StructDefinition &structDefinition,
                      std::map<std::string, const StructDefinition *> &mixinUsage) const;
};
}