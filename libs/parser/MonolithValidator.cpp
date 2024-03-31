#include "MonolithValidator.h"
#include <set>
#include <string>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "Parser.h"
#include "core/Exception.h"
#include "core/St.h"

namespace holgen {
  // TODO: Having separate validator keeps the code clean, but splits the logic
  // and assumes that all plugins will run.
  // Maybe have helper functions that plugins call?
  namespace {
    std::set<std::string> ReservedKeywords{
        "struct",
        "class",
        "namespace",
        "template",
    };

    std::set<std::string> ReservedClassNames{
        St::JsonHelper,
        St::LuaHelper,
        St::GlobalPointer,
        St::FilesystemHelper,
    };

    std::set<std::string> CustomTypes{
        "Ref",
    };

    void EnforceUnique(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                       const AnnotationDefinition &annotationDefinition) {
      THROW_IF(&annotationDefinition != fieldDefinition.GetAnnotation(annotationDefinition.mName),
               "Field {}.{} has multiple {} annotations", structDefinition.mName, fieldDefinition.mName,
               annotationDefinition.mName);
    }

    void EnforceAttributeExists(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                                const AnnotationDefinition &annotationDefinition, const std::string &attributeName) {
      auto attribute = annotationDefinition.GetAttribute(attributeName);
      THROW_IF(attribute == nullptr,
               "Field {}.{} has annotation {} with missing attribute: {}", structDefinition.mName,
               fieldDefinition.mName,
               annotationDefinition.mName, attributeName);
    }

    void EnforceAttributeExists(const StructDefinition &structDefinition,
                                const AnnotationDefinition &annotationDefinition, const std::string &attributeName) {
      auto attribute = annotationDefinition.GetAttribute(attributeName);
      THROW_IF(attribute == nullptr,
               "Struct {} has annotation {} with missing attribute: {}", structDefinition.mName,
               annotationDefinition.mName, attributeName);
    }
  }

  MonolithValidator::MonolithValidator(const ProjectDefinition &projectDefinition) : mProject(projectDefinition) {

  }

  void MonolithValidator::Validate(const StructDefinition &structDefinition) {
    for (const auto &fieldDefinition: structDefinition.mFields) {
      Validate(structDefinition, fieldDefinition);
    }

    for (auto &annotationDefinition: structDefinition.mAnnotations) {
      Validate(structDefinition, annotationDefinition);
    }
  }

  void MonolithValidator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition) {
    for (auto &annotationDefinition: fieldDefinition.mAnnotations) {
      Validate(structDefinition, fieldDefinition, annotationDefinition);
    }
  }

  void MonolithValidator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                           const AnnotationDefinition &annotationDefinition) {
    if (annotationDefinition.mName == Annotations::JsonConvert) {
      EnforceUnique(structDefinition, fieldDefinition, annotationDefinition);
      EnforceAttributeExists(structDefinition, fieldDefinition, annotationDefinition, Annotations::JsonConvert_From);
      EnforceAttributeExists(structDefinition, fieldDefinition, annotationDefinition, Annotations::JsonConvert_Using);
      auto converterName = annotationDefinition.GetAttribute(Annotations::JsonConvert_Using);
      auto convertFrom = annotationDefinition.GetAttribute(Annotations::JsonConvert_From);
      auto convertTo = fieldDefinition.mType;
      auto it = mJsonConverters.find(converterName->mValue.mName);
      if (it == mJsonConverters.end()) {
        mJsonConverters.emplace(converterName->mValue.mName, std::make_pair(convertFrom->mValue, convertTo));
      } else {
        THROW_IF(it->second.first != convertFrom->mValue, "Converter {} cannot convert from multiple types: {} and {}",
                 converterName->mValue.mName, it->second.first.mName, convertFrom->mValue.mName);
        THROW_IF(it->second.second != convertTo, "Converter {} cannot convert to multiple types: {} and {}",
                 converterName->mValue.mName, it->second.second.mName, convertTo.mName);
      }
    } else if (annotationDefinition.mName == Annotations::Id) {
      auto idField = structDefinition.GetIdField();
      THROW_IF(&fieldDefinition != idField, "struct {} has multiple id fields: {} and {}",
               structDefinition.mName, idField->mName, fieldDefinition.mName);
      Type type(mProject, fieldDefinition.mType);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mName), "Field {}.{} uses an invalid type for an id: {}",
               structDefinition.mName, fieldDefinition.mName, fieldDefinition.mType.mName);
    }
  }

  void MonolithValidator::Validate(
      const StructDefinition &structDefinition,
      const AnnotationDefinition &annotationDefinition
  ) {
    if (annotationDefinition.mName == Annotations::Managed) {
      EnforceAttributeExists(structDefinition, annotationDefinition, Annotations::Managed_By);
      EnforceAttributeExists(structDefinition, annotationDefinition, Annotations::Managed_Field);
      auto dataManagerAttribute = annotationDefinition.GetAttribute(Annotations::Managed_By);
      auto dataManager = mProject.GetStruct(dataManagerAttribute->mValue.mName);
      THROW_IF(dataManager == nullptr, "Struct {} references a DataManager {} that does not exist",
               structDefinition.mName, dataManagerAttribute->mValue.mName);
      auto dataManagerAnnotation = dataManager->GetAnnotation(Annotations::DataManager);
      THROW_IF(dataManagerAnnotation == nullptr,
               "Struct {} references a DataManager {} that does not have the {} annotation!",
               structDefinition.mName, dataManager->mName, Annotations::DataManager);
      auto fieldAttribute = annotationDefinition.GetAttribute(Annotations::Managed_Field);
      auto field = dataManager->GetField(fieldAttribute->mValue.mName);
      THROW_IF(field == nullptr, "Struct {} references DataManager field {}.{} that does not exist",
               structDefinition.mName, dataManager->mName, fieldAttribute->mValue.mName);
      auto fieldAnnotation = field->GetAnnotation(Annotations::Container);
      THROW_IF(fieldAnnotation == nullptr, "Struct {} references DataManager field {}.{} that is not a container",
               structDefinition.mName, dataManager->mName, fieldAttribute->mValue.mName);
    }
  }

  void MonolithValidator::Validate() {
    for (const auto &structDefinition: mProject.mStructs) {
      Validate(structDefinition);
    }
  }
}
