#include "ContainerDeleteElemPlugin.h"
#include "generator/utils/CompositeIdHelper.h"
#include "core/St.h"

namespace holgen {
void ContainerDeleteElemPlugin::Run() {
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

void ContainerDeleteElemPlugin::ProcessField(Class &cls, const ClassField &field) const {
  auto &underlyingType = field.mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  const ClassField *underlyingIdField = nullptr;
  if (underlyingClass && underlyingClass->mStruct)
    underlyingIdField = underlyingClass->GetIdField();

  auto method = ClassMethod{Naming().ContainerElemDeleterNameInCpp(*field.mField), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    method.mArguments.emplace_back("key", field.mType.mTemplateParameters.front());
  } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
    method.mArguments.emplace_back("elem", field.mType.mTemplateParameters.front());
  } else {
    if (underlyingIdField)
      method.mArguments.emplace_back("idx", underlyingIdField->mType);
    else
      method.mArguments.emplace_back("idx", Type{"size_t"});
  }

  ProcessMethodVisibility(field, method);
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    ConstructMethodBody(field, method);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

bool ContainerDeleteElemPlugin::ShouldProcess(const ClassField &field) const {
  if (!ContainerPluginBase::ShouldProcess(field))
    return false;
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_None))
    return false;
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName))
    return false;
  return true;
}

void ContainerDeleteElemPlugin::ProcessMethodVisibility(const ClassField &field,
                                                        ClassMethod &method) const {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_Private)) {
    method.mVisibility = Visibility::Private;
  } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                Annotations::Container_Delete,
                                                Annotations::MethodOption_Protected)) {
    method.mVisibility = Visibility::Protected;
  } else {
    method.mExposeToCSharp = true;
    method.mExposeToLua = true;
  }
}

void ContainerDeleteElemPlugin::ConstructMethodBody(const ClassField &field,
                                                    ClassMethod &method) const {
  auto &underlyingType = field.mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  const ClassField *compositeIdField = nullptr;
  if (underlyingClass && underlyingClass->mStruct) {
    compositeIdField = CompositeIdHelper::GetObjectIdField(*underlyingClass, false);
  }

  CodeBlock indexDeleters;
  CodeBlock indexReassigners;

  for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
    auto indexOn = annotation.GetAttribute(Annotations::Index_On);
    auto indexField = underlyingClass->GetFieldFromDefinitionName(indexOn->mValue.mName);
    indexDeleters.Add("{}.erase(ptr->{}());",
                      Naming().FieldIndexNameInCpp(*field.mField, annotation),
                      Naming().FieldGetterNameInCpp(*indexField->mField));
    indexReassigners.Add("{}.at({}.back().{}()) = idx;",
                         Naming().FieldIndexNameInCpp(*field.mField, annotation), field.mName,
                         Naming().FieldGetterNameInCpp(*indexField->mField));
  }

  if (compositeIdField) {
    method.mBody.Add("auto ptr = {}({});",
                     Naming().ContainerElemGetterNameInCpp(*field.mField) +
                         St::CompositeId_RawGetterSuffix,
                     method.mArguments.back().mName);
  }

  if (!indexDeleters.mContents.empty()) {
    if (!compositeIdField) {
      method.mBody.Add("auto ptr = {}({});", Naming().ContainerElemGetterNameInCpp(*field.mField),
                       method.mArguments.back().mName);
    }
    method.mBody.Add(std::move(indexDeleters));
  }

  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    method.mBody.Add("{}.erase(key);", field.mName);
  } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
    method.mBody.Add("{}.erase(elem);", field.mName);
  } else if (compositeIdField) {
    auto versionField = CompositeIdHelper::GetObjectVersionField(*underlyingClass);
    method.mBody.Add("ptr->{}(ptr->{}() + 1);",
                     Naming().FieldSetterNameInCpp(versionField->mField->mName),
                     Naming().FieldGetterNameInCpp(versionField->mField->mName));
    auto idField = CompositeIdHelper::GetObjectIdField(*underlyingClass);
    method.mBody.Add("ptr->{}(-1 - {}{});", Naming().FieldSetterNameInCpp(idField->mField->mName),
                     field.mName, St::CompositeId_NextDeletedIndexSuffix);
    method.mBody.Add("++{}{};", field.mName, St::CompositeId_NextDeletedIndexSuffix);
    method.mBody.Add("{}{} = ptr->{}();", field.mName, St::CompositeId_NextDeletedIndexSuffix,
                     Naming().FieldGetterNameInCpp(idField->mField->mName));
  } else {
    method.mBody.Add("if (size_t(idx) != {}.size() - 1) {{", field.mName);
    method.mBody.Indent(1);
    method.mBody.Add(std::move(indexReassigners));
    method.mBody.Add("{0}[idx] = std::move({0}.back());", field.mName);
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
    method.mBody.Add("{}.pop_back();", field.mName);
  }
}
} // namespace holgen
