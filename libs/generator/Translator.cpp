#include "Translator.h"
#include <map>
#include "core/St.h"
#include "core/Exception.h"
#include "generator/generators/GeneratorJson.h"
#include "generator/generators/GeneratorLua.h"
#include "generator/generators/GeneratorGlobalPointer.h"
#include "TypeInfo.h"
#include "core/Decorators.h"
#include "parser/Validator.h"

namespace holgen {

  TranslatedProject Translator::Translate(const ProjectDefinition &project) {
    THROW_IF(mProject != nullptr, "Translator didnt terminate gracefully last time...");
    mProject = &project;
    Validator(project).Validate();
    TranslatedProject translatedProject(*mProject);
    for (auto &structDefinition: project.mStructs) {
      GenerateClass(translatedProject.mClasses.emplace_back(), structDefinition);
    }

    // Enrichment should not create new classes. It can add new fields though.
    // Enrichers should check StructDefinition for user fields, the ones in Class may be created by enrichment
    GeneratorJson generatorJson(project, translatedProject);
    generatorJson.EnrichClasses();
    GeneratorLua generatorLua(project, translatedProject);
    generatorLua.EnrichClasses();
    GeneratorGlobalPointer generatorGlobalPointer(project, translatedProject);
    generatorGlobalPointer.EnrichClasses();

    // After all integrations processed all real structs, create helpers
    generatorJson.GenerateHelpers();
    generatorLua.GenerateHelpers();
    generatorGlobalPointer.GenerateHelpers();

    mProject = nullptr;
    return translatedProject;
  }

  void Translator::GenerateClass(Class &generatedClass, const StructDefinition &structDefinition) const {
    generatedClass.mName = structDefinition.mName;
    for (auto &fieldDefinition: structDefinition.mFields) {
      ProcessField(generatedClass, fieldDefinition);
    }
    auto managedDecorator = structDefinition.GetDecorator(Decorators::Managed);
    if (managedDecorator != nullptr) {
      auto managedByAttribute = managedDecorator->GetAttribute(Decorators::Managed_By);
      auto managedFieldAttribute = managedDecorator->GetAttribute(Decorators::Managed_Field);
      auto manager = mProject->GetStruct(managedByAttribute->mValue.mName);
      auto managerField = manager->GetField(managedFieldAttribute->mValue.mName);
      auto managerFieldContainerDecorator = managerField->GetDecorator(Decorators::Container);
      auto managerFieldContainerElemNameAttribute = managerFieldContainerDecorator->GetAttribute(
          Decorators::Container_ElemName);
      auto managerFieldContainerConstAttribute = managerFieldContainerDecorator->GetAttribute(
          Decorators::Container_Const);
      auto idField = structDefinition.GetIdField();

      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = "Get";
      getter.mIsStatic = true;
      getter.mIsConst = false;
      getter.mReturnType.mName = structDefinition.mName;
      getter.mReturnType.mType = PassByType::Pointer;
      getter.mReturnType.mIsConst = managerFieldContainerConstAttribute != nullptr;
      auto &idArg = getter.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(idArg.mType, idField->mType);
      idArg.mName = "id";
      getter.mBody.Add("return {}<{}>::GetInstance()->{}(id);",
                       St::GlobalPointerName, manager->mName,
                       St::GetGetterMethodName(managerFieldContainerElemNameAttribute->mValue.mName));
    }
  }

  void Translator::ProcessField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto &generatedField = generatedClass.mFields.emplace_back();

    generatedField.mName = St::GetFieldNameInCpp(fieldDefinition.mName);
    TypeInfo::Get().ConvertToType(generatedField.mType, fieldDefinition.mType);
    bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);

    {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName);
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mReturnType = generatedField.mType;
      getter.mIsConst = true;
      if (!isPrimitive) {
        getter.mReturnType.mIsConst = true;
        getter.mReturnType.mType = PassByType::Reference;
      }
    }

    // non-const getter for non-primitives only
    // TODO: have methods like ShouldFieldHaveSetter and ShouldFieldHaveNonConstGetter
    // Will be useful for AddElem methods too
    if (!isPrimitive) {
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName);
      getter.mBody.Line() << "return " << generatedField.mName << ";";
      getter.mReturnType = generatedField.mType;
      getter.mIsConst = false;
      getter.mReturnType.mType = PassByType::Reference;
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
      setter.mReturnType.mName = "void";
    }

    ProcessContainerField(generatedClass, fieldDefinition);
  }

  void Translator::ProcessContainerField(Class &generatedClass, const FieldDefinition &fieldDefinition) const {
    auto container = fieldDefinition.GetDecorator(Decorators::Container);
    if (!container)
      return;
    bool isConstContainer = container->GetAttribute(Decorators::Container_Const) != nullptr;
    auto elemName = container->GetAttribute(Decorators::Container_ElemName);
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters[0];
    auto underlyingStructDefinition = mProject->GetStruct(underlyingType.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();

    for (auto &dec: fieldDefinition.mDecorators) {
      if (dec.mName != Decorators::Index)
        continue;
      auto indexOn = dec.GetAttribute(Decorators::Index_On);
      auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
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
        TypeInfo::Get().ConvertToType(func.mReturnType, underlyingType);
        func.mReturnType.mIsConst = isConst;
        func.mReturnType.mType = PassByType::Pointer;
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
      // Generate AddElem
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetAdderMethodName(elemName->mValue.mName);
      func.mIsConst = false;
      func.mReturnType.mName = "bool";
      if (isConstContainer)
        func.mVisibility = Visibility::Private;
      else
        func.mVisibility = Visibility::Public;
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
        auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
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
      if (isConstContainer && !isConst)
        continue;
      auto &func = generatedClass.mMethods.emplace_back();
      func.mName = St::GetGetterMethodName(elemName->mValue.mName);
      func.mIsConst = isConst;
      TypeInfo::Get().ConvertToType(func.mReturnType, underlyingType);
      func.mReturnType.mType = PassByType::Pointer;
      func.mReturnType.mIsConst = isConst;
      auto &arg = func.mArguments.emplace_back();
      TypeInfo::Get().ConvertToType(arg.mType, underlyingIdField->mType);
      arg.mType.mIsConst = false;
      arg.mName = "idx";
      {
        auto line = func.mBody.Line();
        line << "if (idx >= " << generatedField.mName << ".size()";
        if (TypeInfo::Get().SignedIntegralTypes.contains(arg.mType.mName)) {
          line << "|| idx < 0";
        }
        line << ")";
      }
      func.mBody.Indent(1);
      func.mBody.Line() << "return nullptr;";
      func.mBody.Indent(-1);
      func.mBody.Line() << "return &" << generatedField.mName << "[idx];";
    }

  }

}
