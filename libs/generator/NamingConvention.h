#pragma once

#include <string>

namespace holgen {
  struct AnnotationDefinition;
  struct FieldDefinition;
  struct StructDefinition;
  struct TranslatedProject;
  struct FunctionDefinition;
  struct Class;

  class NamingConvention {
  public:
    explicit NamingConvention(TranslatedProject &project);
    virtual std::string FieldNameInCpp(const std::string &fieldName) const;
    virtual std::string FieldNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef = false) const;
    virtual std::string FieldNameInLua(const FieldDefinition &fieldDefinition, bool dereferenceRef = false) const;
    virtual std::string FieldIndexNameInCpp(
        const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation) const;
    virtual std::string FieldGetterNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef = false) const;
    virtual std::string FieldGetterNameInCpp(const std::string &fieldName) const;
    virtual std::string VariantGetterNameInCpp(
        const FieldDefinition &fieldDefinition, const StructDefinition &getAs) const;
    virtual std::string ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemExistenceCheckerNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemDeleterNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string ContainerIndexGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                                      const AnnotationDefinition &indexAnnotation) const;
    virtual std::string ManagedClassIndexGetterNameInCpp(const AnnotationDefinition &indexAnnotation) const;
    virtual std::string FieldSetterNameInCpp(const FieldDefinition &fieldDefinition) const;
    virtual std::string FieldSetterNameInCpp(const std::string& fieldName) const;
    virtual std::string LuaFunctionSetterNameInCpp(const FunctionDefinition &functionDefinition) const;
    virtual std::string LuaFunctionCheckerNameInCpp(const FunctionDefinition &functionDefinition) const;
    virtual std::string LuaFunctionHandleNameInCpp(const FunctionDefinition &functionDefinition) const;
    virtual std::string LuaMetatableName(const Class &cls) const;
  private:
    TranslatedProject &mProject;
  };
}
