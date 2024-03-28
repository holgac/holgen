#include "ContainerFieldPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"
#include "../Naming.h"

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
    GenerateContainerNextIndexField(generatedClass, fieldDefinition);
    GenerateContainerAddElem(generatedClass, fieldDefinition, true);
    GenerateContainerAddElem(generatedClass, fieldDefinition, false);
    GenerateContainerGetElem(generatedClass, fieldDefinition);
    if (CanImplementHasElem(generatedClass, fieldDefinition))
      GenerateContainerHasElem(generatedClass, fieldDefinition);
    if (CanImplementDeleteElem(generatedClass, fieldDefinition))
      GenerateContainerDeleteElem(generatedClass, fieldDefinition);
    GenerateContainerGetCount(generatedClass, fieldDefinition);
    // TODO: if annotated, should support deletion (only support integral/string)
    // For maps, deletion is simple
    // For vectors, delete middle element and mark as deleted (so that newly created ones can use it)
  }

  void ContainerFieldPlugin::ProcessContainerIndex(Class &generatedClass, const FieldDefinition &fieldDefinition,
                                                   const AnnotationDefinition &dec) {
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters.back();
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    auto indexOn = dec.GetAttribute(Annotations::Index_On);
    auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
    auto underlyingIdField = underlyingStructDefinition->GetIdField();
    auto &indexField = generatedClass.mFields.emplace_back(
        Naming(mProject).FieldIndexNameInCpp(fieldDefinition, dec), Type{"std::map"});
    auto indexType = dec.GetAttribute(Annotations::Index_Using);
    if (indexType != nullptr) {
      indexField.mType = Type{mProject.mProject, indexType->mValue};
    }

    indexField.mType.mTemplateParameters.emplace_back(mProject.mProject, fieldIndexedOn.mType);
    if (underlyingIdField)
      indexField.mType.mTemplateParameters.emplace_back(mProject.mProject, underlyingIdField->mType);
    else
      indexField.mType.mTemplateParameters.emplace_back("size_t");

    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &func = generatedClass.mMethods.emplace_back(
          Naming(mProject).ContainerIndexGetterNameInCpp(fieldDefinition, dec),
          Type{mProject.mProject, underlyingType, PassByType::Pointer, constness},
          Visibility::Public,
          constness);
      if (i == 0)
        func.mExposeToLua = true;
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
      auto generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
      if (TypeInfo::Get().CppKeyedContainers.contains(generatedField.mType.mName))
        func.mBody.Add("return &{}.at(it->second);", generatedField.mName);
      else
        func.mBody.Add("return &{}[it->second];", generatedField.mName);
    }
  }

  void ContainerFieldPlugin::GenerateContainerAddElem(Class &generatedClass, const FieldDefinition &fieldDefinition,
                                                      bool useMoveRef) {
    auto generatedField = generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    if (useMoveRef && TypeInfo::Get().CppPrimitives.contains(generatedField->mType.mName))
      return;
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters.back();
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    const FieldDefinition *underlyingIdField = nullptr;
    if (underlyingStructDefinition)
      underlyingIdField = underlyingStructDefinition->GetIdField();
    bool isKeyedContainer = TypeInfo::Get().CppKeyedContainers.contains(generatedField->mType.mName);

    auto &func = generatedClass.mMethods.emplace_back(
        Naming(mProject).ContainerElemAdderNameInCpp(fieldDefinition),
        Type{"bool"},
        Visibility::Public,
        Constness::NotConst
    );
    if (useMoveRef) {
      func.mArguments.emplace_back(
          "elem", Type{mProject.mProject, underlyingType, PassByType::MoveReference});
    } else if (TypeInfo::Get().CppPrimitives.contains(generatedField->mType.mName)) {
      func.mArguments.emplace_back(
          "elem", Type{mProject.mProject, underlyingType});
    } else {
      func.mArguments.emplace_back(
          "elem", Type{mProject.mProject, underlyingType, PassByType::Reference, Constness::Const});
    }

    CodeBlock validators;
    CodeBlock inserters;
    for (auto &dec: fieldDefinition.mAnnotations) {
      if (dec.mName != Annotations::Index)
        continue;
      auto indexOn = dec.GetAttribute(Annotations::Index_On);
      auto &fieldIndexedOn = *underlyingStructDefinition->GetField(indexOn->mValue.mName);
      auto indexFieldName = Naming(mProject).FieldIndexNameInCpp(fieldDefinition, dec);
      auto getterMethodName = Naming(mProject).FieldGetterNameInCpp(fieldIndexedOn);
      validators.Add("if ({}.contains(elem.{}())) {{", indexFieldName, getterMethodName);
      validators.Indent(1);
      // TODO: remove exclamation mark
      validators.Add(R"(HOLGEN_WARN("{} with {}={{}} already exists!", elem.{}());)",
                     underlyingStructDefinition->mName, indexOn->mValue.mName, getterMethodName);
      validators.Add("return false;");
      validators.Indent(-1);
      validators.Add("}}");
      inserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
    }
    func.mBody.Add(validators);
    if (isKeyedContainer) {
      func.mBody.Add("auto newId = {}NextId;", generatedField->mName);
      func.mBody.Add("++{}NextId;", generatedField->mName);
    } else if (!inserters.mContents.empty() || underlyingIdField) {
      func.mBody.Line() << "auto newId = " << generatedField->mName << ".size();";
    }
    func.mBody.Add(inserters);
    if (underlyingIdField) {
      func.mArguments.back().mType.mConstness = Constness::NotConst;
      func.mBody.Add("elem.{}(newId);", Naming(mProject).FieldSetterNameInCpp(*underlyingIdField));
    }
    std::string elemToInsert = "elem";
    if (useMoveRef)
      elemToInsert = std::format("std::forward<{}>(elem)", func.mArguments.back().mType.mName);
    if (isKeyedContainer) {
      func.mBody.Add("{}.emplace(newId, {});", generatedField->mName,
                     elemToInsert);
    } else if (TypeInfo::Get().CppSets.contains(generatedField->mType.mName)) {
      func.mBody.Add("{}.emplace({});", generatedField->mName, elemToInsert);
    } else {
      func.mBody.Add("{}.emplace_back({});", generatedField->mName, elemToInsert);
    }
    func.mBody.Line() << "return true;";
  }

  void ContainerFieldPlugin::GenerateContainerGetElem(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto &underlyingType = fieldDefinition.mType.mTemplateParameters.back();
    auto underlyingStructDefinition = mProject.mProject.GetStruct(underlyingType.mName);
    const FieldDefinition *underlyingIdField = nullptr;
    if (underlyingStructDefinition)
      underlyingIdField = underlyingStructDefinition->GetIdField();
    auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    if (TypeInfo::Get().CppSets.contains(generatedField.mType.mName))
      return;
    bool isKeyedContainer = TypeInfo::Get().CppKeyedContainers.contains(generatedField.mType.mName);
    for (int i = 0; i < 2; ++i) {
      auto constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &func = generatedClass.mMethods.emplace_back(
          Naming(mProject).ContainerElemGetterNameInCpp(fieldDefinition),
          Type{mProject.mProject, underlyingType, PassByType::Pointer, constness},
          Visibility::Public,
          constness
      );
      if (i == 0)
        func.mExposeToLua = true;
      bool isSigned = false;
      if (underlyingIdField) {
        auto &arg = func.mArguments.emplace_back("idx", Type{mProject.mProject, underlyingIdField->mType});
        if (TypeInfo::Get().SignedIntegralTypes.contains(arg.mType.mName))
          isSigned = true;
      } else {
        func.mArguments.emplace_back("idx", Type{"size_t"});
      }
      if (isKeyedContainer) {
        func.mBody.Add("auto it = {}.find(idx);", generatedField.mName);
        func.mBody.Add("if (it == {}.end())", generatedField.mName);
      } else {
        auto line = func.mBody.Line();
        line << "if (idx >= " << generatedField.mName << ".size()";
        if (isSigned) {
          line << " || idx < 0";
        }
        line << ")";
      }
      func.mBody.Indent(1);
      func.mBody.Line() << "return nullptr;";
      func.mBody.Indent(-1);
      if (isKeyedContainer) {
        func.mBody.Add("return &it->second;");
      } else {
        func.mBody.Line() << "return &" << generatedField.mName << "[idx];";
      }
    }
  }

  void ContainerFieldPlugin::GenerateContainerGetCount(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto &func = generatedClass.mMethods.emplace_back(
        Naming(mProject).ContainerElemCountNameInCpp(fieldDefinition),
        Type{"size_t"});
    func.mExposeToLua = true;
    func.mBody.Add("return {}.size();", Naming(mProject).FieldNameInCpp(fieldDefinition));
  }

  void ContainerFieldPlugin::GenerateContainerDeleteElem(
      Class &generatedClass, const FieldDefinition &fieldDefinition) {
    // TODO: rename to method everywhere
    auto &func = generatedClass.mMethods.emplace_back(
        Naming(mProject).ContainerElemDeleterNameInCpp(fieldDefinition),
        Type{"void"}, Visibility::Public, Constness::NotConst);
    auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    if (TypeInfo::Get().CppKeyedContainers.contains(generatedField.mType.mName)) {
      func.mArguments.emplace_back("key", Type{mProject.mProject, fieldDefinition.mType.mTemplateParameters.front()});
    } else if (TypeInfo::Get().CppSets.contains(generatedField.mType.mName)) {
      func.mArguments.emplace_back("elem", Type{mProject.mProject, fieldDefinition.mType.mTemplateParameters.front()});
    } else {
      func.mArguments.emplace_back("idx", Type{"size_t"});
    }
    CodeBlock indexDeleters;
    CodeBlock indexReassigners;
    auto underlyingStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters.back().mName);
    for (auto &annotation: fieldDefinition.mAnnotations) {
      if (annotation.mName != Annotations::Index)
        continue;
      auto indexOn = annotation.GetAttribute(Annotations::Index_On);
      auto indexField = underlyingStruct->GetField(indexOn->mValue.mName);
      indexDeleters.Add("{}.erase(ptr->{}());",
                        Naming(mProject).FieldIndexNameInCpp(fieldDefinition, annotation),
                        Naming(mProject).FieldGetterNameInCpp(*indexField));
      indexReassigners.Add("{}.at({}.back().{}()) = idx;",
                           Naming(mProject).FieldIndexNameInCpp(fieldDefinition, annotation),
                           Naming(mProject).FieldNameInCpp(fieldDefinition),
                           Naming(mProject).FieldGetterNameInCpp(*indexField)
      );
    }
    if (!indexDeleters.mContents.empty()) {
      func.mBody.Add("auto ptr = {}({});",
                     Naming(mProject).ContainerElemGetterNameInCpp(fieldDefinition),
                     func.mArguments.back().mName);
      func.mBody.Add(indexDeleters);
    }
    func.mExposeToLua = true;
    if (TypeInfo::Get().CppKeyedContainers.contains(generatedField.mType.mName)) {
      func.mBody.Add("{}.erase(key);", Naming(mProject).FieldNameInCpp(fieldDefinition));
    } else if (TypeInfo::Get().CppSets.contains(generatedField.mType.mName)) {
      func.mBody.Add("{}.erase(elem);", Naming(mProject).FieldNameInCpp(fieldDefinition));
    } else {
      func.mBody.Add("if (idx != {}.size() - 1) {{", Naming(mProject).FieldNameInCpp(fieldDefinition));
      func.mBody.Indent(1);
      func.mBody.Add(indexReassigners);
      func.mBody.Add("{0}[idx] = std::move({0}.back());", Naming(mProject).FieldNameInCpp(fieldDefinition));
      func.mBody.Indent(-1);
      func.mBody.Add("}}");
      func.mBody.Add("{}.pop_back();", Naming(mProject).FieldNameInCpp(fieldDefinition));
    }
  }

  void ContainerFieldPlugin::GenerateContainerHasElem(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto &func = generatedClass.mMethods.emplace_back(
        Naming(mProject).ContainerElemExistenceCheckerNameInCpp(fieldDefinition),
        Type{"bool"});
    func.mExposeToLua = true;
    auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    auto &arg = func.mArguments.emplace_back("elem", Type{mProject.mProject,
                                                          fieldDefinition.mType.mTemplateParameters.front()});
    if (TypeInfo::Get().CppKeyedContainers.contains(generatedField.mType.mName)) {
      arg.mName = "key";
    }
    // TODO: this logic exists in multiple places
    if (!TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
      arg.mType.mType = PassByType::Reference;
      arg.mType.mConstness = Constness::Const;
    }
    func.mBody.Add("return {}.contains({});", Naming(mProject).FieldNameInCpp(fieldDefinition), arg.mName);
  }

  bool ContainerFieldPlugin::CanImplementDeleteElem(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto underlyingStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters.back().mName);
    auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    if (!underlyingStruct || !underlyingStruct->GetIdField())
      return true;
    if (!TypeInfo::Get().CppIndexedContainers.contains(generatedField.mType.mName))
      return true;
    return false;
  }

  bool ContainerFieldPlugin::CanImplementHasElem(Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    return !TypeInfo::Get().CppIndexedContainers.contains(generatedField.mType.mName);
  }

  void ContainerFieldPlugin::GenerateContainerNextIndexField(
      Class &generatedClass, const FieldDefinition &fieldDefinition) {
    auto generatedField = generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));

    if (TypeInfo::Get().CppKeyedContainers.contains(generatedField->mType.mName)) {
      auto underlyingIdField = mProject.mProject.GetStruct(
          fieldDefinition.mType.mTemplateParameters.back().mName)->GetIdField();
      auto &nextIdField = generatedClass.mFields.emplace_back(
          generatedField->mName + "NextId",
          Type{mProject.mProject, underlyingIdField->mType});
      nextIdField.mDefaultValue = "0";
      generatedField = generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
    }

  }
}
