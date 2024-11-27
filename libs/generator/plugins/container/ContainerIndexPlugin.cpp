#include "ContainerIndexPlugin.h"

namespace holgen {
void ContainerIndexPlugin::Run() {
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

void ContainerIndexPlugin::ProcessField(Class &cls, const ClassField &field) {
  for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
    ProcessIndex(cls, field, annotation);
  }
}

void ContainerIndexPlugin::ProcessIndex(Class &cls, const ClassField &field,
                                        const AnnotationDefinition &annotationDefinition) const {
  Validate().IndexAnnotation(cls, field, annotationDefinition);
  GenerateIndexField(cls, field, annotationDefinition);
  GenerateIndexGetter(cls, field, annotationDefinition, Constness::Const);
  GenerateIndexGetter(cls, field, annotationDefinition, Constness::NotConst);
}

void ContainerIndexPlugin::GenerateIndexField(
    Class &cls, const ClassField &field, const AnnotationDefinition &annotationDefinition) const {
  auto &underlyingType = field.mField->mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  auto indexOn = annotationDefinition.GetAttribute(Annotations::Index_On);
  auto &fieldIndexedOn = *underlyingClass->GetFieldFromDefinitionName(indexOn->mValue.mName);
  auto indexField = ClassField{Naming().FieldIndexNameInCpp(*field.mField, annotationDefinition),
                               Type{"std::map"}};
  auto indexType = annotationDefinition.GetAttribute(Annotations::Index_Using);
  if (indexType != nullptr) {
    indexField.mType = Type{mProject, annotationDefinition.mDefinitionSource, indexType->mValue};
  }

  indexField.mType.mTemplateParameters.emplace_back(mProject, field.mField->mDefinitionSource,
                                                    fieldIndexedOn.mField->mType);
  if (auto underlyingIdField = underlyingClass->GetIdField())
    indexField.mType.mTemplateParameters.emplace_back(mProject, field.mField->mDefinitionSource,
                                                      underlyingIdField->mField->mType);
  else
    indexField.mType.mTemplateParameters.emplace_back("size_t");
  Validate().NewField(cls, indexField);
  cls.mFields.push_back(std::move(indexField));
}

void ContainerIndexPlugin::GenerateIndexGetter(Class &cls, const ClassField &field,
                                               const AnnotationDefinition &annotationDefinition,
                                               Constness constness) const {
  auto &underlyingType = field.mField->mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  auto indexOn = annotationDefinition.GetAttribute(Annotations::Index_On);
  auto &fieldIndexedOn = *underlyingClass->GetFieldFromDefinitionName(indexOn->mValue.mName);

  auto method =
      ClassMethod{Naming().ContainerIndexGetterNameInCpp(*field.mField, annotationDefinition),
                  Type{mProject, field.mField->mDefinitionSource, underlyingType,
                       PassByType::Pointer, constness},
                  Visibility::Public, constness};
  if (constness == Constness::Const) {
    method.mExposeToCSharp = true;
    method.mExposeToLua = true;
  }
  method.mArguments.emplace_back(
      "key",
      Type{mProject, fieldIndexedOn.mField->mDefinitionSource, fieldIndexedOn.mField->mType});
  method.mArguments.back().mType.PreventCopying();

  auto indexFieldName = Naming().FieldIndexNameInCpp(*field.mField, annotationDefinition);
  method.mBody.Add("auto it = {}.find(key);", indexFieldName);
  method.mBody.Add("if (it == {}.end())", indexFieldName);
  method.mBody.Indent(1);
  method.mBody.Add("return nullptr;");
  method.mBody.Indent(-1);
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName))
    method.mBody.Add("return &{}.at(it->second);", field.mName);
  else
    method.mBody.Add("return &{}[it->second];", field.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
