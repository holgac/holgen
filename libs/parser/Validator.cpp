#include "Validator.h"
#include <set>
#include <string>
#include "generator/TypeInfo.h"
#include "core/Decorators.h"
#include "Parser.h"
#include "core/Exception.h"

namespace holgen {
  namespace {
    std::set<std::string> ReservedKeywords{
        "struct",
        "class",
        "namespace",
        "template"
    };

    void EnforceUnique(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                       const DecoratorDefinition &decoratorDefinition) {
      THROW_IF(&decoratorDefinition != fieldDefinition.GetDecorator(decoratorDefinition.mName),
               "Field {}.{} has multiple {} decorators", structDefinition.mName, fieldDefinition.mName,
               decoratorDefinition.mName);
    }

    void EnforceDecoratorExists(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                                const std::string &decoratorName) {
      auto decorator = fieldDefinition.GetDecorator(decoratorName);
      THROW_IF(decorator == nullptr,
               "Field {}.{} does not have a decorator {}", structDefinition.mName,
               fieldDefinition.mName, decoratorName);
    }

    void EnforceAttributeExists(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                                const DecoratorDefinition &decoratorDefinition, const std::string &attributeName) {
      auto attribute = decoratorDefinition.GetAttribute(attributeName);
      THROW_IF(attribute == nullptr,
               "Field {}.{} has decorator {} with missing attribute: {}", structDefinition.mName,
               fieldDefinition.mName,
               decoratorDefinition.mName, attributeName);
    }

    void EnforceAttributeExists(const StructDefinition &structDefinition,
                                const DecoratorDefinition &decoratorDefinition, const std::string &attributeName) {
      auto attribute = decoratorDefinition.GetAttribute(attributeName);
      THROW_IF(attribute == nullptr,
               "Struct {} has decorator {} with missing attribute: {}", structDefinition.mName,
               decoratorDefinition.mName, attributeName);
    }
  }

  Validator::Validator(const ProjectDefinition &projectDefinition) : mProject(projectDefinition) {

  }

  void Validator::Validate(const StructDefinition &structDefinition) {
    THROW_IF(ReservedKeywords.contains(structDefinition.mName), "Struct {} uses a reserved keyword.",
             structDefinition.mName);
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(structDefinition.mName), "Struct {} uses a reserved keyword.",
             structDefinition.mName);
    THROW_IF(&structDefinition != mProject.GetStruct(structDefinition.mName), "Duplicate struct name: {}",
             structDefinition.mName)
    for (const auto &fieldDefinition: structDefinition.mFields) {
      Validate(structDefinition, fieldDefinition);
    }

    for (auto &decoratorDefinition: structDefinition.mDecorators) {
      Validate(structDefinition, decoratorDefinition);
    }
  }

  void Validator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition) {
    THROW_IF(ReservedKeywords.contains(fieldDefinition.mName), "Field {}.{} uses a reserved keyword.",
             structDefinition.mName, fieldDefinition.mName);
    THROW_IF(TypeInfo::Get().CppPrimitives.contains(fieldDefinition.mName), "Field {}.{} uses a reserved keyword.",
             structDefinition.mName, fieldDefinition.mName);
    THROW_IF(&fieldDefinition != structDefinition.GetField(fieldDefinition.mName), "Duplicate field name: {}.{}",
             structDefinition.mName, fieldDefinition.mName)
    Validate(structDefinition, fieldDefinition, fieldDefinition.mType);
    for (auto &decoratorDefinition: fieldDefinition.mDecorators) {
      Validate(structDefinition, fieldDefinition, decoratorDefinition);
    }
  }

  void Validator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                           const DecoratorDefinition &decoratorDefinition) {
    if (decoratorDefinition.mName == Decorators::JsonConvert) {
      EnforceUnique(structDefinition, fieldDefinition, decoratorDefinition);
      EnforceAttributeExists(structDefinition, fieldDefinition, decoratorDefinition, Decorators::JsonConvert_From);
      EnforceAttributeExists(structDefinition, fieldDefinition, decoratorDefinition, Decorators::JsonConvert_Using);
      auto converterName = decoratorDefinition.GetAttribute(Decorators::JsonConvert_Using);
      auto convertFrom = decoratorDefinition.GetAttribute(Decorators::JsonConvert_From);
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
    } else if (decoratorDefinition.mName == Decorators::Container) {
      EnforceUnique(structDefinition, fieldDefinition, decoratorDefinition);
      EnforceAttributeExists(structDefinition, fieldDefinition, decoratorDefinition, Decorators::Container_ElemName);
      Type type;
      TypeInfo::Get().ConvertToType(type, fieldDefinition.mType);
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
      auto underlyingManagedDecorator = underlyingStruct->GetDecorator(Decorators::Managed);
      auto underlyingNoLuaDecorator = underlyingStruct->GetDecorator(Decorators::Managed);
      THROW_IF(underlyingManagedDecorator == nullptr && underlyingNoLuaDecorator == nullptr && !TypeInfo::Get().CppStableContainers.contains(type.mName),
               "{}.{} should either be a stable container like deque, or {} should be managed",
               structDefinition.mName, fieldDefinition.mName, underlyingType.mName);
    } else if (decoratorDefinition.mName == Decorators::Index) {
      EnforceDecoratorExists(structDefinition, fieldDefinition, Decorators::Container);
      // double validate to avoid order problems, this validator piece depends on Container being valid.
      Validate(structDefinition, fieldDefinition, *fieldDefinition.GetDecorator(Decorators::Container));
      EnforceAttributeExists(structDefinition, fieldDefinition, decoratorDefinition, Decorators::Index_On);
      auto indexUsing = decoratorDefinition.GetAttribute(Decorators::Index_Using);
      if (indexUsing) {
        EnforceAttributeExists(structDefinition, fieldDefinition, decoratorDefinition, Decorators::Index_Using);
        Type type;
        TypeInfo::Get().ConvertToType(type, indexUsing->mValue);
        THROW_IF(!TypeInfo::Get().CppKeyedContainers.contains(type.mName),
                 "{}.{} uses invalid index type: {}",
                 structDefinition.mName, fieldDefinition.mName, indexUsing->mValue.mName)
      }
      auto indexOn = decoratorDefinition.GetAttribute(Decorators::Index_On);
      auto underlyingStruct = mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
      auto underlyingField = underlyingStruct->GetField(indexOn->mValue.mName);
      THROW_IF(underlyingField == nullptr, "{}.{} indexes on {}.{} which doesn't exist",
               structDefinition.mName, fieldDefinition.mName,
               underlyingStruct->mName, indexOn->mValue.mName
      )
      Type indexType;
      TypeInfo::Get().ConvertToType(indexType, underlyingField->mType);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(indexType.mName),
               "{}.{} indexes on {}.{} which is not a valid key",
               structDefinition.mName, fieldDefinition.mName,
               underlyingStruct->mName, indexOn->mName
      )
      auto indexForConverter = decoratorDefinition.GetAttribute(Decorators::Index_ForConverter);
      THROW_IF(
          indexForConverter != nullptr && structDefinition.GetDecorator(Decorators::DataManager) == nullptr,
          "{} attribute can only be used when the struct is decorated with {}",
          indexForConverter->mName, Decorators::DataManager);
    } else if (decoratorDefinition.mName == Decorators::Id) {
      auto idField = structDefinition.GetIdField();
      THROW_IF(&fieldDefinition != idField, "struct {} has multiple id fields: {} and {}",
               structDefinition.mName, idField->mName, fieldDefinition.mName);
      Type type;
      TypeInfo::Get().ConvertToType(type, fieldDefinition.mType);
      THROW_IF(!TypeInfo::Get().KeyableTypes.contains(type.mName), "Field {}.{} uses an invalid type for an id: {}",
               structDefinition.mName, fieldDefinition.mName, fieldDefinition.mType.mName);
    }
  }

  void Validator::Validate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition,
                           const TypeDefinition &typeDefinition) {
    Type type;
    TypeInfo::Get().ConvertToType(type, typeDefinition);
    THROW_IF(!TypeInfo::Get().CppTypes.contains(type.mName) && mProject.GetStruct(type.mName) == nullptr,
             "Field {}.{} uses an unknown type: {}", structDefinition.mName, fieldDefinition.mName,
             typeDefinition.mName);
    for (const auto &templateParameter : typeDefinition.mTemplateParameters) {
      Validate(structDefinition, fieldDefinition, templateParameter);
    }
  }

  void Validator::Validate(
      const StructDefinition &structDefinition,
      const DecoratorDefinition &decoratorDefinition
  ) {
    if (decoratorDefinition.mName == Decorators::Managed) {
      EnforceAttributeExists(structDefinition, decoratorDefinition, Decorators::Managed_By);
      EnforceAttributeExists(structDefinition, decoratorDefinition, Decorators::Managed_Field);
      auto dataManagerAttribute = decoratorDefinition.GetAttribute(Decorators::Managed_By);
      auto dataManager = mProject.GetStruct(dataManagerAttribute->mValue.mName);
      THROW_IF(dataManager == nullptr, "Struct {} references a DataManager {} that does not exist",
               structDefinition.mName, dataManagerAttribute->mValue.mName);
      auto dataManagerDecorator = dataManager->GetDecorator(Decorators::DataManager);
      THROW_IF(dataManagerDecorator == nullptr,
               "Struct {} references a DataManager {} that does not have the {} decorator!",
               structDefinition.mName, dataManager->mName, Decorators::DataManager);
      auto fieldAttribute = decoratorDefinition.GetAttribute(Decorators::Managed_Field);
      auto field = dataManager->GetField(fieldAttribute->mValue.mName);
      THROW_IF(field == nullptr, "Struct {} references DataManager field {}.{} that does not exist",
               structDefinition.mName, dataManager->mName, fieldAttribute->mValue.mName);
      auto fieldDecorator = field->GetDecorator(Decorators::Container);
      THROW_IF(fieldDecorator == nullptr, "Struct {} references DataManager field {}.{} that is not a container",
               structDefinition.mName, dataManager->mName, fieldAttribute->mValue.mName);
    }
  }

  void Validator::Validate() {
    for (const auto &structDefinition: mProject.mStructs) {
      Validate(structDefinition);
    }

  }
}
