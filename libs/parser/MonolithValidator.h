#pragma once

#include <map>
#include "Parser.h"

namespace holgen {

  class MonolithValidator {
  public:
    MonolithValidator(const ProjectDefinition &projectDefinition);
    void Validate();
  private:
    const ProjectDefinition &mProject;
    void Validate(const StructDefinition &structDefinition);
    void Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition);
    void Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                  const TypeDefinition &typeDefinition);
    void Validate(const StructDefinition &structDefinition, const AnnotationDefinition &annotationDefinition);
    void Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                  const AnnotationDefinition &annotationDefinition);
    std::map<std::string, std::pair<TypeDefinition, TypeDefinition>> mJsonConverters;
    void ValidateContainerAnnotation(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                  const AnnotationDefinition &annotationDefinition);
  };

}
