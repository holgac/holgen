#pragma once

#include <string>
#include <vector>


namespace holgen {
  struct ProjectDefinition;

  struct TypeDefinition {
    std::string mName;
    std::vector<TypeDefinition> mTemplateParameters;
    bool operator==(const TypeDefinition &rhs) const;
  };

  // @annotation(attribute=5)
  struct AnnotationAttributeDefinition {
    std::string mName;
    TypeDefinition mValue;
  };

  struct AnnotationDefinition {
    std::string mName;
    std::vector<AnnotationAttributeDefinition> mAttributes;
    [[nodiscard]] const AnnotationAttributeDefinition *GetAttribute(const std::string &name) const;
  };

  struct FieldDefinition {
    TypeDefinition mType;
    std::string mName;
    std::vector<AnnotationDefinition> mAnnotations;
    std::string mDefaultValue;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;
  };

  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<AnnotationDefinition> mAnnotations;
    [[nodiscard]] const AnnotationDefinition *GetAnnotation(const std::string &name) const;
    [[nodiscard]] const FieldDefinition *GetField(const std::string &name) const;
    [[nodiscard]] const FieldDefinition *GetIdField() const;
  };

  struct ProjectDefinition {
    std::vector<StructDefinition> mStructs;
    [[nodiscard]] const StructDefinition *GetStruct(const std::string &name) const;
  };


}