#pragma once

#include <string>

namespace holgen {
  struct AnnotationDefinition;
  struct FieldDefinition;
  struct TranslatedProject;

  class Naming {
  public:
    // TODO: InCpp suffix is not needed, maybe have CppNaming and LuaNaming classes?
    explicit Naming(TranslatedProject &project);
    std::string FieldNameInCpp(const FieldDefinition &fieldDefinition);
    std::string FieldIndexNameInCpp(
        const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation);
    std::string FieldGetterNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef = false);
    std::string ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition);
  private:
    TranslatedProject &mProject;
  };
}
