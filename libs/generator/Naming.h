#pragma once

#include <string>

namespace holgen {
  struct AnnotationDefinition;
  struct FieldDefinition;
  struct TranslatedProject;
  struct FunctionDefinition;

  class Naming {
  public:
    // TODO: InCpp suffix is not needed, maybe have CppNaming and LuaNaming classes?
    explicit Naming(TranslatedProject &project);
    std::string FieldNameInCpp(const FieldDefinition &fieldDefinition);
    std::string FieldIndexNameInCpp(
        const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation);
    std::string FieldGetterNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef = false);
    std::string ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition);
    std::string ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition);
    std::string ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition);
    std::string ContainerIndexGetterNameInCpp(const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation);
    std::string ManagedClassIndexGetterNameInCpp(const AnnotationDefinition &indexAnnotation);
    std::string FieldSetterNameInCpp(const FieldDefinition &fieldDefinition);
    std::string LuaFunctionSetterNameInCpp(const FunctionDefinition& functionDefinition);
  private:
    TranslatedProject &mProject;
  };
}
