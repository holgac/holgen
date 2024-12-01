#include "ContainerAddElemPlugin.h"
#include "core/St.h"
#include "generator/utils/CompositeIdHelper.h"

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
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName))
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
  auto underlyingIdField =
      mProject.GetClass(field.mType.mTemplateParameters.back().mName)->GetIdField();
  auto nextIdField = ClassField{field.mName + St::Container_NextIdSuffix,
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
  const ClassField *compositeIdField = nullptr;
  if (underlyingClass) {
    underlyingIdField = underlyingClass->GetIdField();
    compositeIdField = CompositeIdHelper::GetObjectIdField(*underlyingClass, false);
    THROW_IF(compositeIdField && compositeIdField != underlyingIdField,
             "Composite id of {} ({}) is not marked with @id!", underlyingClass->mName,
             underlyingClass->mStruct->mDefinitionSource);
  }

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
    GenerateAddElemMethodBody(cls, underlyingClass, method, field, useMoveRef);
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerAddElemPlugin::GenerateIndexInserterAndValidators(const Class &underlyingClass,
                                                                const ClassField &field,
                                                                CodeBlock &indexInserters,
                                                                CodeBlock &validators) const {
  bool isFirst = true;
  for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
    auto indexOn = annotation.GetAttribute(Annotations::Index_On);
    auto &fieldIndexedOn = *underlyingClass.GetFieldFromDefinitionName(indexOn->mValue.mName);
    auto indexFieldName = Naming().FieldIndexNameInCpp(*field.mField, annotation);
    auto getterMethodName = Naming().FieldGetterNameInCpp(*fieldIndexedOn.mField);
    validators.Add("if ({}.contains(elem.{}())) {{", indexFieldName, getterMethodName);
    validators.Indent(1);
    if (isFirst)
      isFirst = false;
    else
      validators.Add(R"(HOLGEN_WARN("{} with {}={{}} already exists", elem.{}());)",
                     underlyingClass.mName, indexOn->mValue.mName, getterMethodName);
    validators.Add("return nullptr;");
    validators.Indent(-1);
    validators.Add("}}");
    indexInserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
  }
}

void ContainerAddElemPlugin::GenerateAddElemMethodBody(const Class &cls,
                                                       const Class *underlyingClass,
                                                       ClassMethod &method, const ClassField &field,
                                                       bool useMoveRef) const {
  const ClassField *underlyingIdField = nullptr;
  const ClassField *compositeIdField = nullptr;
  if (underlyingClass) {
    underlyingIdField = underlyingClass->GetIdField();
    compositeIdField = CompositeIdHelper::GetObjectIdField(*underlyingClass, false);
    THROW_IF(compositeIdField && compositeIdField != underlyingIdField,
             "Composite id of {} ({}) is not marked with @id!", underlyingClass->mName,
             underlyingClass->mStruct->mDefinitionSource);
  }

  CodeBlock indexInserters;
  CodeBlock indexValidators;
  CodeBlock idValidator;
  if (underlyingClass)
    GenerateIndexInserterAndValidators(*underlyingClass, field, indexInserters, indexValidators);
  if (underlyingIdField)
    GenerateIdValidator(idValidator, field, *underlyingIdField);

  method.mBody.Add(std::move(indexValidators));

  std::string elemToInsert =
      useMoveRef ? std::format("std::move(elem)", method.mArguments.back().mType.mName) : "elem";

  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    // it's wasteful to use structs with composite id inside keyed containers, but no need to fail.
    method.mBody.Add("auto newId = {}NextId;", field.mName);
    method.mBody.Add("++{}NextId;", field.mName);
    method.mBody.Add(std::move(idValidator));
    method.mBody.Add(std::move(indexInserters));
    method.mBody.Add("auto[it, res] = {}.emplace(newId, {});", field.mName, elemToInsert);
    method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, \"Corrupt internal ID counter - "
                     "was {}.{} modified externally?\");",
                     cls.mName, field.mField->mName);
    method.mBody.Add("auto& newElem = it->second;");
    if (underlyingIdField)
      method.mBody.Add("newElem.{}(newId);",
                       Naming().FieldSetterNameInCpp(underlyingIdField->mField->mName));
    method.mBody.Add("return &newElem;", field.mName);
  } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
    method.mBody.Add("auto[it, res] = {}.emplace({});", field.mName, elemToInsert);
    method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, \"Attempting to insert duplicate "
                     "element to {}.{}!\");",
                     cls.mName, field.mField->mName);
    method.mBody.Add("return &(*it);", field.mName);
    method.mReturnType.mConstness = Constness::Const;
  } else if (compositeIdField) {
    method.mBody.Add("if ({}{} > 0) {{", field.mName, St::CompositeId_NextDeletedIndexSuffix);
    method.mBody.Indent(1);

    method.mBody.Add("auto newId = {}{} - 1;", field.mName, St::CompositeId_NextDeletedIndexSuffix);
    method.mBody.Add(idValidator);
    method.mBody.Add(indexInserters);
    method.mBody.Add("{}{} = -1 - {}[newId].{}();", field.mName,
                     St::CompositeId_NextDeletedIndexSuffix, field.mName,
                     Naming().FieldGetterNameInCpp(*compositeIdField->mField));
    auto objectVersionField = CompositeIdHelper::GetObjectVersionField(*underlyingClass);
    method.mBody.Add("auto version = {}[newId].{}();;", field.mName,
                     Naming().FieldGetterNameInCpp(objectVersionField->mField->mName));
    method.mBody.Add("{}[newId] = {};", field.mName, elemToInsert);
    method.mBody.Add("auto &newElem = {}[newId];", field.mName);
    method.mBody.Add("newElem.{}(version);",
                     Naming().FieldSetterNameInCpp(objectVersionField->mField->mName));
    method.mBody.Add("newElem.{}(newId);",
                     Naming().FieldSetterNameInCpp(compositeIdField->mField->mName));
    method.mBody.Add("return &newElem;");

    method.mBody.Indent(-1);
    method.mBody.Add("}} else {{");
    method.mBody.Indent(1);

    method.mBody.Add("auto newId = {}.size();", field.mName);
    method.mBody.Add(std::move(idValidator));
    method.mBody.Add(std::move(indexInserters));
    method.mBody.Add("auto &newElem = {}.emplace_back({});", field.mName, elemToInsert);
    method.mBody.Add("newElem.{}(newId);",
                     Naming().FieldSetterNameInCpp(compositeIdField->mField->mName));
    method.mBody.Add("return &newElem;");

    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  } else if (!indexInserters.mContents.empty() || underlyingIdField) {
    method.mBody.Add("auto newId = {}.size();", field.mName);
    method.mBody.Add(std::move(idValidator));
    method.mBody.Add(std::move(indexInserters));
    method.mBody.Add("auto &newElem = {}.emplace_back({});", field.mName, elemToInsert);
    if (underlyingIdField)
      method.mBody.Add("newElem.{}(newId);",
                       Naming().FieldSetterNameInCpp(underlyingIdField->mField->mName));
    method.mBody.Add("return &newElem;");
  } else {
    method.mBody.Add("return &{}.emplace_back({});", field.mName, elemToInsert);
  }
}

void ContainerAddElemPlugin::GenerateIdValidator(CodeBlock &codeBlock, const ClassField &field,
                                                 const ClassField &underlyingIdField) const {
  auto &underlyingType = field.mType.mTemplateParameters.back();
  codeBlock.Add("auto idInElem = elem.{}();",
                Naming().FieldGetterNameInCpp(*underlyingIdField.mField));
  codeBlock.Add(
      "HOLGEN_FAIL_IF(idInElem != {0}::IdType(-1) && idInElem != {0}::IdType(newId), \"Objects "
      "not loaded in the right order!\");",
      underlyingType.mName);
}
} // namespace holgen
