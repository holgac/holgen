#include "NamingConvention.h"
#include <format>
#include "parser/ProjectDefinition.h"
#include "core/St.h"
#include "TranslatedProject.h"
#include "core/Annotations.h"

namespace holgen {
  NamingConvention::NamingConvention(TranslatedProject &project) : mProject(project) {}

  std::string NamingConvention::FieldNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef) const {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingClass->GetIdField() && !dereferenceRef) {
        return FieldNameInCpp(fieldDefinition.mName + "Id");
      }
    }
    return FieldNameInCpp(fieldDefinition.mName);
  }

  std::string NamingConvention::FieldNameInLua(const FieldDefinition &fieldDefinition, bool dereferenceRef) const {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingClass->GetIdField() && !dereferenceRef) {
        return std::format("{}Id", fieldDefinition.mName);
      }
    }
    return fieldDefinition.mName;
  }

  std::string NamingConvention::FieldIndexNameInCpp(
      const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation) const {
    auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
    return std::format("{}{}Index", FieldNameInCpp(fieldDefinition), St::Capitalize(indexOn->mValue.mName));
  }

  std::string
  NamingConvention::FieldGetterNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef) const {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingClass = mProject.GetClass(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingClass->GetIdField() && !dereferenceRef) {
        return std::format("Get{}Id", St::Capitalize(fieldDefinition.mName));
      }
    }
    return FieldGetterNameInCpp(fieldDefinition.mName);
  }

  std::string
  NamingConvention::FieldGetterNameInCpp(const std::string &fieldName) const {
    return std::format("Get{}", St::Capitalize(fieldName));
  }

  std::string NamingConvention::ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition) const {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Get{}", St::Capitalize(elemName->mValue.mName));
  }

  std::string NamingConvention::ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition) const {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Add{}", St::Capitalize(elemName->mValue.mName));
  }

  std::string NamingConvention::ContainerElemExistenceCheckerNameInCpp(const FieldDefinition &fieldDefinition) const {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Has{}", St::Capitalize(elemName->mValue.mName));
  }

  std::string NamingConvention::ContainerElemDeleterNameInCpp(const FieldDefinition &fieldDefinition) const {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Delete{}", St::Capitalize(elemName->mValue.mName));
  }

  std::string NamingConvention::ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition) const {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Get{}Count", St::Capitalize(elemName->mValue.mName));
  }

  std::string NamingConvention::ContainerIndexGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                                              const AnnotationDefinition &indexAnnotation) const {
    auto elemName = fieldDefinition.GetAnnotation(Annotations::Container)->GetAttribute(
        Annotations::Container_ElemName);
    auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
    return std::format("Get{}From{}", St::Capitalize(elemName->mValue.mName), St::Capitalize(indexOn->mValue.mName));
  }

  std::string NamingConvention::ManagedClassIndexGetterNameInCpp(const AnnotationDefinition &indexAnnotation) const {
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

  std::string NamingConvention::LuaFunctionSetterNameInCpp(const FunctionDefinition &functionDefinition) const {
    return std::format("Set{}LuaFunc", functionDefinition.mName);
  }

  std::string NamingConvention::LuaFunctionCheckerNameInCpp(const FunctionDefinition &functionDefinition) const {
    return std::format("Has{}LuaFunc", functionDefinition.mName);
  }

  std::string NamingConvention::LuaFunctionHandleNameInCpp(const FunctionDefinition &functionDefinition) const {
    return "mLuaFuncHandle_" + functionDefinition.mName;
  }

  std::string NamingConvention::LuaMetatableName(const Class &cls) const {
    return cls.mName + "Meta";
  }

  std::string NamingConvention::FieldNameInCpp(const std::string &fieldName) const {
    return std::format("m{}", St::Capitalize(fieldName));
  }

  std::string NamingConvention::VariantGetterNameInCpp(
      const FieldDefinition &fieldDefinition, const StructDefinition &getAs) const {
    return std::format("Get{}As{}", St::Capitalize(fieldDefinition.mName), St::Capitalize(getAs.mName));
  }

  std::string NamingConvention::VariantDestructorNameInCpp(const FieldDefinition &fieldDefinition) const {
    return std::format("Destroy{}", St::Capitalize(fieldDefinition.mName));
  }
}
