#include "Validator.h"
#include <set>
#include <string>
#include "generator/TypeInfo.h"
#include "core/Annotations.h"
#include "Parser.h"
#include "core/Exception.h"
#include "core/St.h"

namespace holgen {
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

    void EnforceAnnotationExists(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                                 const std::string &annotationName) {
      auto annotation = fieldDefinition.GetAnnotation(annotationName);
      THROW_IF(annotation == nullptr,
               "Field {}.{} does not have a annotation {}", structDefinition.mName,
               fieldDefinition.mName, annotationName);
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

  Validator::Validator(const ProjectDefinition &projectDefinition) : mProject(projectDefinition) {

  }

  void Validator::Validate(const StructDefinition &structDefinition) {
    THROW_IF(ReservedKeywords.contains(structDefinition.mName), "Struct {} uses a reserved keyword.",
             structDefinition.mName);
    THROW_IF(ReservedClassNames.contains(structDefinition.mName), "Struct {} uses a reserved class name.",
             structDefinition.mName);
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(structDefinition.mName), "Struct {} uses a reserved keyword.",
             structDefinition.mName);
    THROW_IF(&structDefinition != mProject.GetStruct(structDefinition.mName), "Duplicate struct name: {}",
             structDefinition.mName)
    for (const auto &fieldDefinition: structDefinition.mFields) {
      Validate(structDefinition, fieldDefinition);
    }

    for (auto &annotationDefinition: structDefinition.mAnnotations) {
      Validate(structDefinition, annotationDefinition);
    }
  }

  void Validator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition) {
    THROW_IF(ReservedKeywords.contains(fieldDefinition.mName), "Field {}.{} uses a reserved keyword.",
             structDefinition.mName, fieldDefinition.mName);
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(fieldDefinition.mName), "Field {}.{} uses a reserved keyword.",
             structDefinition.mName, fieldDefinition.mName);
    THROW_IF(&fieldDefinition != structDefinition.GetField(fieldDefinition.mName), "Duplicate field name: {}.{}",
             structDefinition.mName, fieldDefinition.mName);
    Validate(structDefinition, fieldDefinition, fieldDefinition.mType);
    if (fieldDefinition.mType.mName == "Ref") {
      THROW_IF(fieldDefinition.mType.mTemplateParameters.size() != 1,
               "Ref field {}.{} should have a single template parameter",
               structDefinition.mName, fieldDefinition.mName);
      auto referencedType = mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      THROW_IF(referencedType == nullptr,
               "Ref field {}.{} references {} which is not a struct defined in this project",
               structDefinition.mName, fieldDefinition.mName, fieldDefinition.mType.mTemplateParameters[0].mName);
      auto managedAnnotation = referencedType->GetAnnotation(Annotations::Managed);
      THROW_IF(managedAnnotation == nullptr,
               "Ref field {}.{} references {} which is not managed",
               structDefinition.mName, fieldDefinition.mName, fieldDefinition.mType.mTemplateParameters[0].mName);
    }

    for (auto &annotationDefinition: fieldDefinition.mAnnotations) {
      Validate(structDefinition, fieldDefinition, annotationDefinition);
    }
  }

  void Validator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
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
    } else if (annotationDefinition.mName == Annotations::Container) {
      EnforceUnique(structDefinition, fieldDefinition, annotationDefinition);
      EnforceAttributeExists(structDefinition, fieldDefinition, annotationDefinition, Annotations::Container_ElemName);
      auto type = Type{fieldDefinition.mType};
      THROW_IF(!TypeInfo::Get().CppIndexedContainers.contains(type.mName), "{}.{} is not a valid indexed container",
               structDefinition.mName, fieldDefinition.mName);
      THROW_IF(fieldDefinition.mType.mTemplateParameters.size() != 1, "{}.{} should have a single template parameter",
               structDefinition.mName, fieldDefinition.mName);
      auto &underlyingType = fieldDefinition.mType.mTemplateParameters[0];
      auto underlyingStruct = mProject.GetStruct(underlyingType.mName);
      THROW_IF(underlyingStruct == nullptr, "{}.{} is a container of {} which is not a user type",
               structDefinition.mName, fieldDefinition.mName, underlyingType.mName);
      auto underlyingIdField = underlyingStruct->GetIdField();
      THROW_IF(underlyingIdField == nullptr,
               "{}.{} is a container of {} which does not have an id field",
               structDefinition.mName, fieldDefinition.mName, underlyingType.mName);
      auto underlyingManagedAnnotation = underlyingStruct->GetAnnotation(Annotations::Managed);
      auto underlyingNoLuaAnnotation = underlyingStruct->GetAnnotation(Annotations::Managed);
      THROW_IF(underlyingManagedAnnotation == nullptr && underlyingNoLuaAnnotation == nullptr &&
               !TypeInfo::Get().CppStableContainers.contains(type.mName),
               "{}.{} should either be a stable container like deque, or {} should be managed",
               structDefinition.mName, fieldDefinition.mName, underlyingType.mName);
    } else if (annotationDefinition.mName == Annotations::Index) {
      EnforceAnnotationExists(structDefinition, fieldDefinition, Annotations::Container);
      // double validate to avoid order problems, this validator piece depends on Container being valid.
      Validate(structDefinition, fieldDefinition, *fieldDefinition.GetAnnotation(Annotations::Container));
      EnforceAttributeExists(structDefinition, fieldDefinition, annotationDefinition, Annotations::Index_On);
      auto indexUsing = annotationDefinition.GetAttribute(Annotations::Index_Using);
      if (indexUsing) {
        EnforceAttributeExists(structDefinition, fieldDefinition, annotationDefinition, Annotations::Index_Using);
        auto type = Type{indexUsing->mValue};
        THROW_IF(!TypeInfo::Get().CppKeyedContainers.contains(type.mName),
                 "{}.{} uses invalid index type: {}",
                 structDefinition.mName, fieldDefinition.mName, indexUsing->mValue.mName)
      }
      auto indexOn = annotationDefinition.GetAttribute(Annotations::Index_On);
      auto underlyingStruct = mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      auto underlyingField = underlyingStruct->GetField(indexOn->mValue.mName);
      THROW_IF(underlyingField == nullptr, "{}.{} indexes on {}.{} which doesn't exist",
               structDefinition.mName, fieldDefinition.mName,
               underlyingStruct->mName, indexOn->mValue.mName
      )
      Type indexType(underlyingField->mType);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(indexType.mName),
               "{}.{} indexes on {}.{} which is not a valid key",
               structDefinition.mName, fieldDefinition.mName,
               underlyingStruct->mName, indexOn->mName
      )
      auto indexForConverter = annotationDefinition.GetAttribute(Annotations::Index_ForConverter);
      THROW_IF(
          indexForConverter != nullptr && structDefinition.GetAnnotation(Annotations::DataManager) == nullptr,
          "{} attribute can only be used when the struct is decorated with {}",
          indexForConverter->mName, Annotations::DataManager);
    } else if (annotationDefinition.mName == Annotations::Id) {
      auto idField = structDefinition.GetIdField();
      THROW_IF(&fieldDefinition != idField, "struct {} has multiple id fields: {} and {}",
               structDefinition.mName, idField->mName, fieldDefinition.mName);
      Type type(fieldDefinition.mType);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mName), "Field {}.{} uses an invalid type for an id: {}",
               structDefinition.mName, fieldDefinition.mName, fieldDefinition.mType.mName);
    }
  }

  void Validator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                           const TypeDefinition &typeDefinition) {
    Type type(typeDefinition);
    THROW_IF(!TypeInfo::Get().CppTypes.contains(type.mName) && !CustomTypes.contains(type.mName)
             && mProject.GetStruct(type.mName) == nullptr
             && mProject.GetEnum(type.mName) == nullptr,
             "Field {}.{} uses an unknown type: {}", structDefinition.mName, fieldDefinition.mName,
             typeDefinition.mName);
    for (const auto &templateParameter : typeDefinition.mTemplateParameters) {
      Validate(structDefinition, fieldDefinition, templateParameter);
    }
  }

  void Validator::Validate(
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

  void Validator::Validate() {
    for (const auto &structDefinition: mProject.mStructs) {
      Validate(structDefinition);
    }

  }
}
