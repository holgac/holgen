#pragma once

#include <string>

namespace holgen {
  struct AnnotationDefinition;
  struct FieldDefinition;
  struct TranslatedProject;
  struct FunctionDefinition;

  class NamingConvention {
  public:
    explicit NamingConvention(TranslatedProject &project);
    virtual std::string FieldNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef = false) const;
    virtual std::string FieldNameInLua(const FieldDefinition &fieldDefinition, bool dereferenceRef = false) const;
    virtual std::string FieldIndexNameInCpp(
        const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation) const;
    virtual std::string FieldGetterNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef = false) const;
    virtual std::string ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemExistenceCheckerNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemDeleterNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerIndexGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                                      const AnnotationDefinition &indexAnnotation) const;
    virtual std::string ManagedClassIndexGetterNameInCpp(const AnnotationDefinition &indexAnnotation) const;
    virtual std::string FieldSetterNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string LuaFunctionSetterNameInCpp(const FunctionDefinition &functionDefinition) const;
  private:
    TranslatedProject &mProject;
  };
}