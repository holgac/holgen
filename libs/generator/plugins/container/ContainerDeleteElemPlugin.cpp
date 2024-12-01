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
  if (underlyingClass && CompositeIdHelper::GetObjectIdField(*underlyingClass, false)) {
    GenerateElemDeleter(cls, field,
                        Naming().ContainerElemDeleterNameInCpp(*field.mField) +
                            St::CompositeId_RawIdxSuffix);
    GenerateElemDeleterForCompositeId(cls, field);
  } else {
    GenerateElemDeleter(cls, field, Naming().ContainerElemDeleterNameInCpp(*field.mField));
  }
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

void ContainerDeleteElemPlugin::GenerateElemDeleterForCompositeId(Class &cls,
                                                                  const ClassField &field) const {
  auto &underlyingType = field.mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  auto compositeIdType = CompositeIdHelper::GetCompositeIdType(mProject, *underlyingClass);
  auto method = ClassMethod{Naming().ContainerElemDeleterNameInCpp(*field.mField), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back(
      "id", Type{compositeIdType->mName, PassByType::Reference, Constness::Const});
  auto idIdField = CompositeIdHelper::GetIdIdField(*compositeIdType);
  auto idVersionField = CompositeIdHelper::GetIdVersionField(*compositeIdType);
  auto objectVersionField = CompositeIdHelper::GetObjectVersionField(*underlyingClass);
  method.mBody.Add(
      "HOLGEN_WARN_AND_RETURN_IF(size_t(id.{}()) >= {}.size(), void(), \"Out of bounds index "
      "when deleting {}.{}\");",
      Naming().FieldGetterNameInCpp(idIdField->mField->mName), field.mName, cls.mName,
      underlyingClass->mName);
  method.mBody.Add(
      "HOLGEN_WARN_AND_RETURN_IF({}[id.{}()].{}() != id.{}(), void(), \"version mismatch "
      "when deleting {}.{}\");",
      field.mName, Naming().FieldGetterNameInCpp(idIdField->mField->mName),
      Naming().FieldGetterNameInCpp(idVersionField->mField->mName),
      Naming().FieldGetterNameInCpp(objectVersionField->mField->mName), cls.mName,
      underlyingClass->mName);
  method.mBody.Add("{}{}(id.{}());", Naming().ContainerElemDeleterNameInCpp(*field.mField),
                   St::CompositeId_RawIdxSuffix,
                   Naming().FieldGetterNameInCpp(idIdField->mField->mName));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerDeleteElemPlugin::GenerateElemDeleter(Class &cls, const ClassField &field,
                                                    const std::string &methodName) const {
  auto &underlyingType = field.mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  const ClassField *underlyingIdField = nullptr;
  if (underlyingClass && underlyingClass->mStruct)
    underlyingIdField = underlyingClass->GetIdField();
  auto method = ClassMethod{methodName, Type{"void"}, Visibility::Public, Constness::NotConst};
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
                         St::CompositeId_RawIdxSuffix,
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
    method.mBody.Add("auto newId = -1 - {}{};", field.mName, St::CompositeId_NextDeletedIndexSuffix);
    method.mBody.Add("{}{} = ptr->{}() + 1;", field.mName, St::CompositeId_NextDeletedIndexSuffix,
                     Naming().FieldGetterNameInCpp(idField->mField->mName));
    method.mBody.Add("ptr->{}(newId);", Naming().FieldSetterNameInCpp(idField->mField->mName),
                     field.mName, St::CompositeId_NextDeletedIndexSuffix);
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
