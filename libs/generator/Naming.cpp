#include "Naming.h"
#include <format>
#include "parser/ProjectDefinition.h"
#include "core/St.h"
#include "TranslatedProject.h"
#include "core/Annotations.h"

namespace holgen {
  Naming::Naming(TranslatedProject &project) : mProject(project) {}

  std::string Naming::FieldNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef) {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingStruct->GetIdField() != nullptr && !dereferenceRef) {
        return std::format("m{}Id", St::Capitalize(fieldDefinition.mName));
      }
    }
    return std::format("m{}", St::Capitalize(fieldDefinition.mName));
  }

  std::string Naming::FieldNameInLua(const FieldDefinition &fieldDefinition, bool dereferenceRef) {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingStruct->GetIdField() != nullptr && !dereferenceRef) {
        return std::format("{}Id", fieldDefinition.mName);
      }
    }
    return fieldDefinition.mName;
  }

  std::string Naming::FieldIndexNameInCpp(
      const FieldDefinition &fieldDefinition, const AnnotationDefinition &indexAnnotation) {
    auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
    return std::format("{}{}Index", FieldNameInCpp(fieldDefinition), St::Capitalize(indexOn->mValue.mName));
  }

  std::string Naming::FieldGetterNameInCpp(const FieldDefinition &fieldDefinition, bool dereferenceRef) {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingStruct->GetIdField() != nullptr && !dereferenceRef) {
        return std::format("Get{}Id", St::Capitalize(fieldDefinition.mName));
      }
    }
    return std::format("Get{}", St::Capitalize(fieldDefinition.mName));
  }

  std::string Naming::ContainerElemGetterNameInCpp(const FieldDefinition &fieldDefinition) {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Get{}", St::Capitalize(elemName->mValue.mName));
  }

  std::string Naming::ContainerElemAdderNameInCpp(const FieldDefinition &fieldDefinition) {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Add{}", St::Capitalize(elemName->mValue.mName));
  }

  std::string Naming::ContainerElemCountNameInCpp(const FieldDefinition &fieldDefinition) {
    auto containerAnnotation = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = containerAnnotation->GetAttribute(Annotations::Container_ElemName);
    return std::format("Get{}Count", St::Capitalize(elemName->mValue.mName));
  }

  std::string Naming::ContainerIndexGetterNameInCpp(const FieldDefinition &fieldDefinition,
                                                    const AnnotationDefinition &indexAnnotation) {
    auto elemName = fieldDefinition.GetAnnotation(Annotations::Container)->GetAttribute(
        Annotations::Container_ElemName);
    auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
    return std::format("Get{}From{}", St::Capitalize(elemName->mValue.mName), St::Capitalize(indexOn->mValue.mName));
  }

  std::string Naming::ManagedClassIndexGetterNameInCpp(const AnnotationDefinition &indexAnnotation) {
    auto indexOn = indexAnnotation.GetAttribute(Annotations::Index_On);
    return std::format("GetFrom{}", St::Capitalize(indexOn->mValue.mName));
  }

  std::string Naming::FieldSetterNameInCpp(const FieldDefinition &fieldDefinition) {
    if (fieldDefinition.mType.mName == "Ref") {
      auto underlyingStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      if (underlyingStruct->GetIdField() != nullptr) {
        return std::format("Set{}Id", St::Capitalize(fieldDefinition.mName));
      }
    }
    return std::format("Set{}", St::Capitalize(fieldDefinition.mName));
  }

  std::string Naming::LuaFunctionSetterNameInCpp(const FunctionDefinition &functionDefinition) {
    return std::format("Set{}LuaFunc", functionDefinition.mName);
  }
}
