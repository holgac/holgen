#include "Translator.h"
#include <map>
#include "core/St.h"
#include "core/Exception.h"
#include "GeneratorJson.h"
#include "TypeInfo.h"
#include "GeneratorLua.h"
#include "Decorators.h"
#include "parser/Validator.h"

namespace holgen {

  TranslatedProject Translator::Translate(const ProjectDefinition &project) {
    THROW_IF(mProject != nullptr, "Translator didnt terminate gracefully last time...");
    mProject = &project;
    Validator(project).Validate();
    TranslatedProject translatedProject;
    std::map<std::string, size_t> classMap;
    for (auto &structDefinition: project.mStructs) {
      auto[it, res] = classMap.try_emplace(structDefinition.mName, translatedProject.mClasses.size());
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

    ProcessContainerField(generatedClass, fieldDefinition);
  }

  void Translator::ProcessContainerField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto container = fieldDefinition.GetDecorator(Decorators::Container);
    if (!container)
      return;
    auto elemName = container->GetAttribute(Decorators::Container_ElemName);
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters[0];
    auto underlyingStructDefinition = mProject->GetStruct(underlyingType.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();

    for (auto &dec: fieldDefinition.mDecorators) {
      if (dec.mName != Decorators::Index)
        continue;
      auto indexOn = dec.GetAttribute(Decorators::Index_On);
      auto& fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
      auto &indexField = generatedClass.mFields.emplace_back();
      indexField.mName = St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName);
      auto indexType = dec.GetAttribute(Decorators::Index_Using);
      if (indexType != nullptr) {
        TypeInfo::Get().ConvertToType(indexField.mType, indexType->mValue);
      } else {
        indexField.mType.mName = "std::map";
      }

      auto &indexKey = indexField.mType.mTemplateParameters.emplace_back();
      TypeInfo::Get().ConvertToType(indexKey, fieldIndexedOn.mType);
      auto &indexValue = indexField.mType.mTemplateParameters.emplace_back();
      TypeInfo::Get().ConvertToType(indexValue, underlyingIdField->mType);

      for (int i = 0; i < 2; ++i) {
        bool isConst = i == 0;
        auto &func = generatedClass.mMethods.emplace_back();
        func.mName = St::GetIndexGetterName(elemName->mValue.mName, indexOn->mValue.mName);
        func.mIsConst = isConst;
        TypeInfo::Get().ConvertToType(func.mType, underlyingType);
        func.mType.mIsConst = isConst;
        func.mType.mType = PassByType::Pointer;
        auto &arg = func.mArguments.emplace_back();
        arg.mName = "key";
        TypeInfo::Get().ConvertToType(arg.mType, fieldIndexedOn.mType);
        if (!TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
          arg.mType.mIsConst = true;
          arg.mType.mType = PassByType::Reference;
        }

        func.mBody.Add("auto it = {}.find(key);", indexField.mName);
        func.mBody.Add("if (it == {}.end())", indexField.mName);
        func.mBody.Indent(1);
        func.mBody.Add("return nullptr;");
        func.mBody.Indent(-1);
        func.mBody.Add("return &{}[it->second];", St::GetFieldNameInCpp(fieldDefinition.mName));

      }
    }

    // Getting generatedField here because we added a new field above which would've invalidated it
    auto &generatedField = *generatedClass.GetField(St::GetFieldNameInCpp(fieldDefinition.mName));
    {
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetAdderMethodName(elemName->mValue.mName);
      func.mIsConst = false;
      func.mType.mName = "bool";
      auto &arg = func.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(arg.mType, underlyingType);
      arg.mType.mIsConst = false;
      arg.mType.mType = PassByType::MoveReference;
      arg.mName = "elem";

      func.mBody.Line() << "auto newId = " << generatedField.mName << ".size();";
      CodeBlock validators;
      CodeBlock inserters;
      for (auto &dec: fieldDefinition.mDecorators) {
        if (dec.mName != Decorators::Index)
          continue;
        auto indexOn = dec.GetAttribute(Decorators::Index_On);
        auto& fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
        auto indexFieldName = St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName);
        auto getterMethodName = St::GetGetterMethodName(fieldIndexedOn.mName);
        validators.Add("if({}.contains(elem.{}()))", indexFieldName, getterMethodName);
        // TODO: some logging mechanism for all these failures?
        validators.Indent(1);
        validators.Add("return false;");
        validators.Indent(-1);
        inserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
      }
      func.mBody.Add(validators);
      func.mBody.Add(inserters);
      func.mBody.Add("{}.emplace_back(std::forward<{}>(elem));", generatedField.mName, arg.mType.mName);
      func.mBody.Add("{}.back().{}(newId);", generatedField.mName, St::GetSetterMethodName(underlyingIdField->mName));
      func.mBody.Line() << "return true;";
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
