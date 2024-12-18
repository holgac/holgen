#include "NamingConvention.h"
#include <format>
#include "generator/TranslatedProject.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "parser/ProjectDefinition.h"

namespace holgen {
NamingConvention::NamingConvention(const TranslatedProject &project) : mProject(project) {}

std::string NamingConvention::FieldNameInCpp(const FieldDefinition &fieldDefinition,
                                             bool dereferenceRef) const {
  if (fieldDefinition.mType.mName == "Ref") {
    auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
    if (underlyingClass->GetIdField() && !dereferenceRef) {
      return FieldNameInCpp(fieldDefinition.mName + "Id");
    }
  }
  return FieldNameInCpp(fieldDefinition.mName);
}

std::string NamingConvention::FieldNameInLua(const FieldDefinition &fieldDefinition,
                                             bool dereferenceRef) const {
  if (fieldDefinition.mType.mName == "Ref") {
    auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
    if (underlyingClass->GetIdField() && !dereferenceRef) {
      return std::format("{}Id", fieldDefinition.mName);
    }
  }
  return fieldDefinition.mName;
}

std::string
    NamingConvention::FieldIndexNameInCpp(const FieldDefinition &fieldDefinition,
                                          const AnnotationDefinition &indexAnnotation) const {
  auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
  return std::format("{}{}Index", FieldNameInCpp(fieldDefinition),
                     St::Capitalize(indexOn->mValue.mName));
}

std::string NamingConvention::FieldGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                                   bool dereferenceRef) const {
  if (fieldDefinition.mType.mName == "Ref") {
    auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
    if (underlyingClass->GetIdField() && !dereferenceRef) {
      return std::format("Get{}Id", St::Capitalize(fieldDefinition.mName));
    }
  }
  return FieldGetterNameInCpp(fieldDefinition.mName);
}

std::string NamingConvention::EnumClassForEachMacro(const Class &cls) const {
  std::string prefix;
  if (!cls.mNamespace.empty())
    prefix = St::Replace(cls.mNamespace, "::", "_") + "_";
  return std::format("{}FOR_EACH_{}", prefix, cls.mName);
}

std::string NamingConvention::EnumClassSwitchMacro(const Class &cls) const {
  std::string prefix;
  if (!cls.mNamespace.empty())
    prefix = St::Replace(cls.mNamespace, "::", "_") + "_";
  return std::format("{}SWITCH_{}", prefix, cls.mName);
}

std::string NamingConvention::EnumClassForEachDoerMacro(const Class &cls) const {
  return EnumClassForEachMacro(cls) + "_DOER";
}

std::string NamingConvention::FieldGetterNameInCpp(const std::string &fieldName) const {
  return std::format("Get{}", St::Capitalize(fieldName));
}

std::string
    NamingConvention::ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition) const {
  auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
  auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
  return std::format("Get{}", St::Capitalize(elemName->mValue.mName));
}

std::string
    NamingConvention::ContainerElemSetterNameInCpp(const FieldDefinition &fieldDefinition) const {
  auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
  auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
  return std::format("Set{}", St::Capitalize(elemName->mValue.mName));
}

std::string
    NamingConvention::ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition) const {
  auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
  auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
  return std::format("Add{}", St::Capitalize(elemName->mValue.mName));
}

std::string NamingConvention::ContainerElemExistenceCheckerNameInCpp(
    const FieldDefinition &fieldDefinition) const {
  auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
  auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
  return std::format("Has{}", St::Capitalize(elemName->mValue.mName));
}

std::string
    NamingConvention::ContainerElemDeleterNameInCpp(const FieldDefinition &fieldDefinition) const {
  auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
  auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
  return std::format("Delete{}", St::Capitalize(elemName->mValue.mName));
}

std::string
    NamingConvention::ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition) const {
  auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
  auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
  return std::format("Get{}Count", St::Capitalize(elemName->mValue.mName));
}

std::string NamingConvention::ContainerIndexGetterNameInCpp(
    const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation) const {
  auto elemName = fieldDefinition.GetAnnotation(Annotations::Container)
                      ->GetAttribute(Annotations::Container_ElemName);
  auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
  return std::format("Get{}From{}", St::Capitalize(elemName->mValue.mName),
                     St::Capitalize(indexOn->mValue.mName));
}

std::string NamingConvention::ManagedClassIndexGetterNameInCpp(
    const AnnotationDefinition &indexAnnotation) const {
  auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
  return std::format("GetFrom{}", St::Capitalize(indexOn->mValue.mName));
}

std::string NamingConvention::FieldSetterNameInCpp(const FieldDefinition &fieldDefinition) const {
  if (fieldDefinition.mType.mName == "Ref") {
    auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
    if (underlyingClass->GetIdField()) {
      return std::format("Set{}Id", St::Capitalize(fieldDefinition.mName));
    }
  }
  return FieldSetterNameInCpp(fieldDefinition.mName);
}

std::string NamingConvention::FieldSetterNameInCpp(const std::string &fieldName) const {
  return std::format("Set{}", St::Capitalize(fieldName));
}

std::string NamingConvention::LuaFunctionSetterNameInCpp(
    const FunctionDefinition &functionDefinition) const {
  return std::format("Set{}LuaFunc", functionDefinition.mName);
}

std::string NamingConvention::LuaFunctionCheckerNameInCpp(
    const FunctionDefinition &functionDefinition) const {
  return std::format("Has{}LuaFunc", functionDefinition.mName);
}

std::string NamingConvention::LuaFunctionHandleNameInCpp(
    const FunctionDefinition &functionDefinition) const {
  return "mLuaFuncHandle_" + functionDefinition.mName;
}

std::string NamingConvention::LuaMetatableName(const std::string &name) const {
  return name;
}

std::string NamingConvention::LuaMetatableName(const Class &cls) const {
  return LuaMetatableName(cls.mName);
}

std::string NamingConvention::LuaMethodCaller(const ClassMethod &method) const {
  if (method.mFunction &&
      method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_OverloadSuffix)) {
    auto methodSuffix =
        method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_OverloadSuffix)
            ->mValue.mName;
    return LuaMethodCaller(method.mName + methodSuffix);
  }
  return LuaMethodCaller(method.mName);
}

std::string NamingConvention::LuaMethodCaller(const std::string &methodName) const {
  return methodName + "CallerFromLua";
}

std::string
    NamingConvention::LuaPublisherCallbacksTableName(const std::string &functionName) const {
  return functionName + "_Callbacks";
}

std::string NamingConvention::CSharpMethodName(const ClassMethod &method) const {
  return St::Capitalize(method.mName);
}

std::string NamingConvention::CSharpMethodDelegateName(const Class &cls,
                                                       const ClassMethod &method) const {
  return CSharpMethodDelegateName(cls.mName, method.mName);
}

std::string NamingConvention::CSharpMethodDelegateName(const std::string &clsName,
                                                       const std::string &methodName) const {
  return St::Capitalize(clsName) + St::Capitalize(methodName) + St::CSharpDelegateSuffix;
}

std::string NamingConvention::CSharpMethodPointerName(const ClassMethod &method) const {
  return CSharpMethodPointerName(method.mName);
}

std::string NamingConvention::CSharpMethodPointerName(const std::string &methodName) const {
  return std::format("_{}Impl", St::Uncapitalize(methodName));
}

std::string NamingConvention::CWrapperName(const Class &cls, const ClassMethod &method) const {
  return std::format("{}_{}_{}", St::Replace(cls.mNamespace, "::", "_"), cls.mName, method.mName);
}

std::string NamingConvention::FieldNameInCSharp(const std::string &fieldName) const {
  return St::Capitalize(fieldName);
}

std::string NamingConvention::PrivateFieldNameInCSharp(const std::string &fieldName) const {
  return std::format("_{}", St::Uncapitalize(fieldName));
}

std::string NamingConvention::ModuleInterfaceNameInCSharp(const std::string &moduleName) const {
  return std::format("I{}", moduleName);
}

std::string NamingConvention::FieldNameInCpp(const std::string &fieldName) const {
  return std::format("m{}", St::Capitalize(fieldName));
}

std::string NamingConvention::VariantGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                                     const StructDefinition &getAs) const {
  return std::format("Get{}As{}", St::Capitalize(fieldDefinition.mName),
                     St::Capitalize(getAs.mName));
}

std::string NamingConvention::VariantResetterNameInCpp(const std::string &fieldName) const {
  return std::format("Reset{}", St::Capitalize(fieldName));
}
} // namespace holgen
