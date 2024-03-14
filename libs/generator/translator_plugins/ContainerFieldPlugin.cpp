#include "ContainerFieldPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
  void ContainerFieldPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        if (fieldDefinition.GetAnnotation(Annotations::Container) == nullptr)
          continue;
        ProcessContainerField(generatedClass, fieldDefinition);
      }
    }
  }

  void ContainerFieldPlugin::ProcessContainerField(
      Class &generatedClass, const FieldDefinition &fieldDefinition
  ) {
    for (auto &annotation: fieldDefinition.mAnnotations) {
      if (annotation.mName != Annotations::Index)
        continue;
      ProcessContainerIndex(generatedClass, fieldDefinition, annotation);
    }
    GenerateContainerAddElem(generatedClass, fieldDefinition);
    GenerateContainerGetElem(generatedClass, fieldDefinition);
  }

  void ContainerFieldPlugin::ProcessContainerIndex(Class &generatedClass, const FieldDefinition &fieldDefinition,
                                                   const AnnotationDefinition &dec) {
    auto elemName = fieldDefinition.GetAnnotation(Annotations::Container)->GetAttribute(
        Annotations::Container_ElemName);
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters.back();
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    auto indexOn = dec.GetAttribute(Annotations::Index_On);
    auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();
    auto &indexField = generatedClass.mFields.emplace_back(
        St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName), Type{"std::map"});
    auto indexType = dec.GetAttribute(Annotations::Index_Using);
    if (indexType != nullptr) {
      indexField.mType = Type{mProject.mProject, indexType->mValue};
    }

    indexField.mType.mTemplateParameters.emplace_back(mProject.mProject, fieldIndexedOn.mType);
    indexField.mType.mTemplateParameters.emplace_back(mProject.mProject, underlyingIdField->mType);

    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &func = generatedClass.mMethods.emplace_back(
          St::GetIndexGetterName(elemName->mValue.mName, indexOn->mValue.mName),
          Type{mProject.mProject, underlyingType, PassByType::Pointer, constness},
          Visibility::Public,
          constness);
      auto &arg = func.mArguments.emplace_back(
          "key", Type{mProject.mProject, fieldIndexedOn.mType});
      if (!TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
        arg.mType.mConstness = Constness::Const;
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

  void ContainerFieldPlugin::GenerateContainerAddElem(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto container = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = container->GetAttribute(Annotations::Container_ElemName);
    bool isConstContainer = container->GetAttribute(Annotations::Container_Const) != nullptr;
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters.back();
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();
    auto &generatedField = *generatedClass.GetField(St::GetFieldNameInCpp(fieldDefinition.mName));

    auto &func = generatedClass.mMethods.emplace_back(
        St::GetAdderMethodName(elemName->mValue.mName),
        Type{"bool"},
        isConstContainer ? Visibility::Private : Visibility::Public,
        Constness::NotConst
    );
    auto &arg = func.mArguments.emplace_back("elem",
                                             Type{mProject.mProject, underlyingType, PassByType::MoveReference});

    func.mBody.Line() << "auto newId = " << generatedField.mName << ".size();";
    CodeBlock validators;
    CodeBlock inserters;
    for (auto &dec: fieldDefinition.mAnnotations) {
      if (dec.mName != Annotations::Index)
        continue;
      auto indexOn = dec.GetAttribute(Annotations::Index_On);
      auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
      auto indexFieldName = St::GetIndexFieldName(fieldDefinition.mName, indexOn->mValue.mName);
      auto getterMethodName = St::GetGetterMethodName(fieldIndexedOn.mName);
      validators.Add("if ({}.contains(elem.{}())) {{", indexFieldName, getterMethodName);
      validators.Indent(1);
      validators.Add(R"(HOLGEN_WARN("{} with {}={{}} already exists!", elem.{}());)",
                     underlyingStructDefinition->mName, indexOn->mValue.mName, getterMethodName);
      validators.Add("return false;");
      validators.Indent(-1);
      validators.Add("}}");
      inserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
    }
    func.mBody.Add(validators);
    func.mBody.Add(inserters);
    func.mBody.Add("elem.{}(newId);", St::GetSetterMethodName(underlyingIdField->mName));
    func.mBody.Add("{}.emplace_back(std::forward<{}>(elem));", generatedField.mName, arg.mType.mName);
    func.mBody.Add("{}.back().{}(newId);", generatedField.mName,
                   St::GetSetterMethodName(underlyingIdField->mName));
    func.mBody.Line() << "return true;";
  }

  void ContainerFieldPlugin::GenerateContainerGetElem(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto container = fieldDefinition.GetAnnotation(Annotations::Container);
    auto elemName = container->GetAttribute(Annotations::Container_ElemName);
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters.back();
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();
    auto &generatedField = *generatedClass.GetField(St::GetFieldNameInCpp(fieldDefinition.mName));
    for (int i = 0; i < 2; ++i) {
      auto constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &func = generatedClass.mMethods.emplace_back(
          St::GetGetterMethodName(elemName->mValue.mName),
          Type{mProject.mProject, underlyingType, PassByType::Pointer, constness},
          Visibility::Public,
          constness
      );
      auto &arg = func.mArguments.emplace_back("idx", Type{mProject.mProject, underlyingIdField->mType});
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
