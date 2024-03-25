#pragma once

#include <string>

namespace holgen {
  struct AnnotationDefinition;
  struct FieldDefinition;
  struct TranslatedProject;

  class Naming {
  public:
    Naming(TranslatedProject &project);
    std::string FieldNameInCpp(const FieldDefinition &fieldDefinition);
    std::string FieldIndexNameInCpp(const FieldDefinition &fieldDefinition, const AnnotationDefinition& indexAnnotation);
  private:
    TranslatedProject &mProject;
  };
}
