#include "ContainerAddElemPlugin.h"

namespace holgen {
void ContainerAddElemPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    for (auto &field: cls.mFields) {
      if (!ShouldProcess(field))
        continue;
      ProcessField(cls, field);
    }
  }
}

void ContainerAddElemPlugin::ProcessField(Class &cls, const ClassField &field) const {
  auto container = field.mField->GetAnnotation(Annotations::Container);
  Validate().ContainerAnnotation(cls, field, *container);
  GenerateNextIndexField(cls, field);
  GenerateAddElemMethod(cls, field, true);
  GenerateAddElemMethod(cls, field, false);
}

bool ContainerAddElemPlugin::ShouldProcess(const ClassField &field) const {
  if (!ContainerPluginBase::ShouldProcess(field))
    return false;
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName))
    return false;
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                         Annotations::MethodOption_None))
    return false;
  return true;
}

void ContainerAddElemPlugin::GenerateNextIndexField(Class &cls, const ClassField &field) const {
  if (!TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName))
    return;
  auto underlyingIdField =
      mProject.GetClass(field.mType.mTemplateParameters.back().mName)->GetIdField();
  auto nextIdField = ClassField{field.mName + "NextId",
                                Type{mProject, underlyingIdField->mField->mDefinitionSource,
                                     underlyingIdField->mField->mType}};
  nextIdField.mDefaultValue = "0";
  Validate().NewField(cls, nextIdField);
  cls.mFields.push_back(std::move(nextIdField));
}

void ContainerAddElemPlugin::GenerateAddElemMethod(Class &cls, const ClassField &field,
                                                   bool useMoveRef) const {
  auto &underlyingType = field.mType.mTemplateParameters.back();
  bool isPointer = underlyingType.mType == PassByType::Pointer;
  if (useMoveRef && (isPointer || TypeInfo::Get().CppPrimitives.contains(underlyingType.mName))) {
    return;
  }
  if (!useMoveRef && !isPointer && !field.mType.SupportsCopy(mProject)) {
    return;
  }
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  const ClassField *underlyingIdField = nullptr;
  if (underlyingClass) {
    underlyingIdField = underlyingClass->GetIdField();
  }
  bool isKeyedContainer = TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName);

  auto method = ClassMethod{Naming().ContainerElemAdderNameInCpp(*field.mField),
                            Type{underlyingType.mName, PassByType::Pointer}, Visibility::Public,
                            Constness::NotConst};
  method.mArguments.emplace_back("elem", underlyingType);
  if (useMoveRef) {
    method.mArguments.back().mType.mType = PassByType::MoveReference;
  } else if (!isPointer) {
    method.mArguments.back().mType.PreventCopying();
  }
  if (underlyingIdField) {
    method.mArguments.back().mType.mConstness = Constness::NotConst;
  }

  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                         Annotations::MethodOption_Private)) {
    method.mVisibility = Visibility::Private;
  } else if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                                Annotations::MethodOption_Protected)) {
    method.mVisibility = Visibility::Protected;
  } else if (!useMoveRef) {
    // TODO: change to useMoveRef after implementing ReadFromLua
    method.mExposeToCSharp = true;
    method.mExposeToLua = true;
  }
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    CodeBlock validators;
    CodeBlock indexInserters;
    bool isFirst = true;
    for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
      auto indexOn = annotation.GetAttribute(Annotations::Index_On);
      auto &fieldIndexedOn = *underlyingClass->GetFieldFromDefinitionName(indexOn->mValue.mName);
      auto indexFieldName = Naming().FieldIndexNameInCpp(*field.mField, annotation);
      auto getterMethodName = Naming().FieldGetterNameInCpp(*fieldIndexedOn.mField);
      validators.Add("if ({}.contains(elem.{}())) {{", indexFieldName, getterMethodName);
      validators.Indent(1);
      if (isFirst)
        isFirst = false;
      else
        validators.Add(R"(HOLGEN_WARN("{} with {}={{}} already exists", elem.{}());)",
                       underlyingClass->mName, indexOn->mValue.mName, getterMethodName);
      validators.Add("return nullptr;");
      validators.Indent(-1);
      validators.Add("}}");
      indexInserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
    }
    method.mBody.Add(std::move(validators));

    if (isKeyedContainer) {
      method.mBody.Add("auto newId = {}NextId;", field.mName);
      method.mBody.Add("++{}NextId;", field.mName);
    } else if (!indexInserters.mContents.empty() || underlyingIdField) {
      method.mBody.Add("auto newId = {}.size();", field.mName);
    }

    if (underlyingIdField) {
      method.mBody.Add("auto idInElem = elem.{}();",
                       Naming().FieldGetterNameInCpp(*underlyingIdField->mField));
      method.mBody.Add(
          "HOLGEN_FAIL_IF(idInElem != {0}::IdType(-1) && idInElem != {0}::IdType(newId), \"Objects "
          "not loaded in the right order!\");",
          underlyingType.mName);
      method.mBody.Add("elem.{}(newId);",
                       Naming().FieldSetterNameInCpp(*underlyingIdField->mField));
    }

    method.mBody.Add(std::move(indexInserters));

    std::string elemToInsert = "elem";
    if (useMoveRef)
      elemToInsert = std::format("std::forward<{}>(elem)", method.mArguments.back().mType.mName);

    if (isKeyedContainer) {
      method.mBody.Add("auto[it, res] = {}.emplace(newId, {});", field.mName, elemToInsert);
      method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, \"Corrupt internal ID counter - "
                       "was {}.{} modified externally?\");",
                       cls.mName, field.mField->mName);
      method.mBody.Add("return &(it->second);", field.mName);
    } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
      method.mBody.Add("auto[it, res] = {}.emplace({});", field.mName, elemToInsert);
      method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, \"Attempting to insert duplicate "
                       "element to {}\");",
                       field.mField->mName);
      method.mBody.Add("return &(*it);", field.mName);
      method.mReturnType.mConstness = Constness::Const;
    } else if (isPointer) {
      method.mBody.Add("return {}.emplace_back({});", field.mName, elemToInsert);
    } else {
      method.mBody.Add("return &({}.emplace_back({}));", field.mName, elemToInsert);
    }
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
