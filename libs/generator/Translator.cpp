#include "Translator.h"
#include <map>
#include "core/St.h"
#include "core/Exception.h"
#include "GeneratorJson.h"
#include "TypeInfo.h"
#include "GeneratorLua.h"
#include "Decorators.h"

namespace holgen {

  TranslatedProject Translator::Translate(const ProjectDefinition &project) {
    THROW_IF(mProject != nullptr, "Translator didnt terminate gracefully last time...");
    mProject = &project;
    TranslatedProject translatedProject;
    std::map<std::string, size_t> classMap;
    for (auto &structDefinition: project.mStructs) {
      auto[it, res] = classMap.try_emplace(structDefinition.mName, translatedProject.mClasses.size());
      THROW_IF(!res, "Duplicate class: \"{}\"", structDefinition.mName)
      GenerateClass(translatedProject.mClasses.emplace_back(), structDefinition);
    }

    // Enrichment should not create new classes. It can add new fields though.
    // Enrichers should check StructDefinition for user fields, the ones in Class may be created by enrichment
    GeneratorJson generatorJson(project, translatedProject);
    generatorJson.EnrichClasses();
    GeneratorLua generatorLua(project, translatedProject);
    generatorLua.EnrichClasses();

    // After all integrations processed all real structs, create helpers
    generatorJson.GenerateHelpers();
    generatorLua.GenerateHelpers();

    mProject = nullptr;
    return translatedProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const {
    generatedClass.mName = structDefinition.mName;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }
  }

  void Translator::ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto &generatedField = generatedClass.mFields.emplace_back();

    generatedField.mName = fieldDefinition.GetNameInCpp();
    TypeInfo::Get().ConvertToType(generatedField.mType, fieldDefinition.mType);
    bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);

    {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName);
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mType = generatedField.mType;
      getter.mIsConst = true;
      if (!isPrimitive) {
        getter.mType.mIsConst = true;
        getter.mType.mType = PassByType::Reference;
      }
    }

    // non-const getter for non-primitives only
    if (!isPrimitive) {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName);
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mType = generatedField.mType;
      getter.mIsConst = false;
      getter.mType.mType = PassByType::Reference;
    }

    {
      auto &setter = generatedClass.mMethods.emplace_back();
      setter.mName = St::GetSetterMethodName(fieldDefinition.mName);
      setter.mIsConst = false;
      auto &arg = setter.mArguments.emplace_back();
      arg.mType = generatedField.mType;
      if (!isPrimitive) {
        arg.mType.mIsConst = true;
        arg.mType.mType = PassByType::Reference;
      }
      arg.mName = "val";
      setter.mBody.Line() << generatedField.mName << " = val;";
      setter.mType.mName = "void";
    }

    ProcessContainerField(generatedClass, generatedField, fieldDefinition);
  }

  void Translator::ProcessContainerField(Class &generatedClass, const ClassField &generatedField,
                                         const FieldDefinition &fieldDefinition) const {
    auto container = fieldDefinition.GetDecorator(Decorators::Container);
    if (!container)
      return;
    auto elemName = container->GetAttribute(Decorators::Container_ElemName);
    THROW_IF(
        elemName == nullptr,
        "{}.{} has incomplete container definition: Containers should have an {} defined",
        generatedClass.mName, fieldDefinition.mName, Decorators::Container_ElemName)
    THROW_IF(!TypeInfo::Get().CppIndexedContainers.contains(generatedField.mType.mName),
             "{}.{} is not a valid indexed container!",
             generatedClass.mName, fieldDefinition.mName)
    THROW_IF(fieldDefinition.mType.mTemplateParameters.size() != 1,
             "{}.{} should have a single template parameter!",
             generatedClass.mName, fieldDefinition.mName)
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters[0];
    auto underlyingStructDefinition = mProject->GetStruct(underlyingType.mName);
    THROW_IF(underlyingStructDefinition == nullptr,
             "{}.{} is a container of {} which is not a user type!",
             generatedClass.mName, fieldDefinition.mName, underlyingType.mName)
    auto underlyingIdField = underlyingStructDefinition->GetIdField();
    THROW_IF(underlyingIdField == nullptr,
             "{}.{} is a container of {} which does not have an id field!",
             generatedClass.mName, fieldDefinition.mName, underlyingType.mName)
    {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetAdderMethodName(elemName->mValue.mName);
      func.mIsConst = false;
      auto &arg = func.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(arg.mType, underlyingType);
      arg.mType.mIsConst = false;
      arg.mType.mType = PassByType::MoveReference;
      arg.mName = "elem";
      func.mBody.Line() << generatedField.mName << ".emplace_back(std::forward<" << arg.mType.mName << ">(elem));";
      func.mBody.Line() << generatedField.mName << ".back()." << St::GetSetterMethodName(underlyingIdField->mName)
                        << "(" << generatedField.mName << ".size() - 1);";
    }

    for (int i = 0; i < 2; ++i) {
      bool isConst = i == 0;
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetGetterMethodName(elemName->mValue.mName);
      func.mIsConst = isConst;
      TypeInfo::Get().ConvertToType(func.mType, underlyingType);
      func.mType.mType = PassByType::Reference;
      func.mType.mIsConst = isConst;
      auto &arg = func.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(arg.mType, underlyingIdField->mType);
      arg.mType.mIsConst = false;
      arg.mName = "idx";
      func.mBody.Line() << "return " << generatedField.mName << "[idx];";
    }
  }

  ClassField *Class::GetField(const std::string &name) {
    for (auto &field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }
}
