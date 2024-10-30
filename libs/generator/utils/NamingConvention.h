#pragma once
#include <string>

namespace holgen {
struct AnnotationDefinition;
struct FieldDefinition;
struct StructDefinition;
struct TranslatedProject;
struct FunctionDefinition;
struct Class;
struct ClassMethod;

class NamingConvention {
public:
  explicit NamingConvention(const TranslatedProject &project);
  virtual std::string FieldNameInCpp(const std::string &fieldName) const;
  virtual std::string FieldNameInCpp(const FieldDefinition &fieldDefinition,
                                     bool dereferenceRef = false) const;
  virtual std::string FieldNameInLua(const FieldDefinition &fieldDefinition,
                                     bool dereferenceRef = false) const;
  virtual std::string FieldIndexNameInCpp(const FieldDefinition &fieldDefinition,
                                          const AnnotationDefinition &indexAnnotation) const;
  virtual std::string FieldGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                           bool dereferenceRef = false) const;
  virtual std::string FieldGetterNameInCpp(const std::string &fieldName) const;
  virtual std::string VariantGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                             const StructDefinition &getAs) const;
  virtual std::string VariantResetterNameInCpp(const std::string &fieldName) const;
  virtual std::string ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string ContainerElemSetterNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string
      ContainerElemExistenceCheckerNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string ContainerElemDeleterNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string
      ContainerIndexGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                    const AnnotationDefinition &indexAnnotation) const;
  virtual std::string
      ManagedClassIndexGetterNameInCpp(const AnnotationDefinition &indexAnnotation) const;
  virtual std::string FieldSetterNameInCpp(const FieldDefinition &fieldDefinition) const;
  virtual std::string FieldSetterNameInCpp(const std::string &fieldName) const;
  virtual std::string
      LuaFunctionSetterNameInCpp(const FunctionDefinition &functionDefinition) const;
  virtual std::string
      LuaFunctionCheckerNameInCpp(const FunctionDefinition &functionDefinition) const;
  virtual std::string
      LuaFunctionHandleNameInCpp(const FunctionDefinition &functionDefinition) const;
  virtual std::string LuaMetatableName(const Class &cls) const;
  virtual std::string LuaMethodCaller(const ClassMethod &method) const;
  virtual std::string CSharpMethodName(const ClassMethod &method) const;
  virtual std::string CSharpMethodDelegateName(const Class &cls, const ClassMethod &method) const;
  virtual std::string CSharpMethodDelegateName(const std::string &clsName,
                                               const std::string &methodName) const;
  virtual std::string CSharpMethodPointerName(const ClassMethod &method) const;
  virtual std::string CSharpMethodPointerName(const std::string &methodName) const;
  virtual std::string CWrapperName(const Class &cls, const ClassMethod &method) const;
  virtual std::string FieldNameInCSharp(const std::string &fieldName) const;
  virtual std::string PrivateFieldNameInCSharp(const std::string &fieldName) const;
  virtual std::string ModuleInterfaceNameInCSharp(const std::string &moduleName) const;

private:
  const TranslatedProject &mProject;
};
} // namespace holgen
