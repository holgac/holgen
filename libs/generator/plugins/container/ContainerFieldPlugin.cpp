#include "ContainerFieldPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/utils/CompositeIdHelper.h"

namespace holgen {
// TODO: if the class has at least one const container, implement locking
void ContainerFieldPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    for (auto it = cls.mFields.begin(); it != cls.mFields.end(); ++it) {
      if (!it->mField || !it->mField->GetAnnotation(Annotations::Container))
        continue;
      ProcessField(cls, *it);
    }
  }
}

void ContainerFieldPlugin::ProcessField(Class &cls, ClassField &field) {
  auto container = field.mField->GetAnnotation(Annotations::Container);
  Validate().ContainerAnnotation(cls, field, *container);
  auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
  if (underlyingClass && underlyingClass->mStruct &&
      underlyingClass->mStruct->GetAnnotation(Annotations::CompositeId))
    ProcessFieldWithCompositeId(cls, field);

  if (CanImplementHasElem(cls, field))
    GenerateHasElem(cls, field);
  if (CanImplementDeleteElem(cls, field))
    GenerateDeleteElem(cls, field);
  if (CanImplementSetElem(cls, field))
    GenerateSetElem(cls, field);
  GenerateGetCount(cls, field);
}

void ContainerFieldPlugin::ProcessFieldWithCompositeId(Class &cls, const ClassField &field) const {
  auto deletedCountField =
      ClassField{field.mName + St::CompositeId_DeletedCountSuffix, Type{"uint32_t"}};
  deletedCountField.mDefaultValue = "0";
  Validate().NewField(cls, deletedCountField);
  cls.mFields.push_back(std::move(deletedCountField));

  auto deletedIndexField =
      ClassField{field.mName + St::CompositeId_NextDeletedIndexSuffix, Type{"int32_t"}};
  deletedIndexField.mDefaultValue = "-1";
  Validate().NewField(cls, deletedIndexField);
  cls.mFields.push_back(std::move(deletedIndexField));
}

void ContainerFieldPlugin::GenerateGetCount(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Count,
                                         Annotations::MethodOption_None))
    return;
  auto method = ClassMethod{Naming().ContainerElemCountNameInCpp(*field.mField), Type{"size_t"}};

  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Count,
                                         Annotations::MethodOption_Private)) {
    method.mVisibility = Visibility::Private;
  } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                Annotations::Container_Count,
                                                Annotations::MethodOption_Protected)) {
    method.mVisibility = Visibility::Protected;
  } else {
    method.mExposeToCSharp = true;
    method.mExposeToLua = true;
  }
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Count,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    method.mBody.Add("return {}.size();", field.mName);
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerFieldPlugin::GenerateDeleteElem(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_None)) {
    return;
  }

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
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    CodeBlock indexDeleters;
    CodeBlock indexReassigners;
    auto underlyingClass = mProject.GetClass(field.mType.mTemplateParameters.back().mName);
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

    if (!indexDeleters.mContents.empty()) {
      auto compositeIdType = CompositeIdHelper::GetCompositeIdType(mProject, *underlyingClass);
      if (compositeIdType)
        method.mBody.Add("auto ptr = {}({});",
                         Naming().ContainerElemGetterNameInCpp(*field.mField) +
                             St::CompositeId_RawGetterSuffix,
                         method.mArguments.back().mName);
      else
        method.mBody.Add("auto ptr = {}({});", Naming().ContainerElemGetterNameInCpp(*field.mField),
                         method.mArguments.back().mName);
      method.mBody.Add(std::move(indexDeleters));
    }

    if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
      method.mBody.Add("{}.erase(key);", field.mName);
    } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
      method.mBody.Add("{}.erase(elem);", field.mName);
    } else {
      std::string idxExpression = "idx";
      if (underlyingIdField &&
          TypeInfo::Get().SignedIntegralTypes.contains(underlyingIdField->mType.mName)) {
        // double cast to avoid sign extension; zero padding should be slightly faster
        idxExpression = std::format("size_t({}(idx))",
                                    TypeInfo::Get().GetUnsigned(underlyingIdField->mType.mName));
      }
      method.mBody.Add("if ({} != {}.size() - 1) {{", idxExpression, field.mName);
      method.mBody.Indent(1);
      method.mBody.Add(std::move(indexReassigners));
      method.mBody.Add("{0}[{1}] = std::move({0}.back());", field.mName, idxExpression);
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
      method.mBody.Add("{}.pop_back();", field.mName);
    }
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerFieldPlugin::GenerateSetElem(Class &cls, const ClassField &field) {
  auto method = ClassMethod{Naming().ContainerElemSetterNameInCpp(*field.mField), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  auto fixedSizeEnumArray = mProject.mProject.GetEnum(field.mField->mType.mArraySize);
  method.mArguments.emplace_back("idx", Type{fixedSizeEnumArray->mName});
  method.mArguments.back().mType.PreventCopying();
  method.mArguments.emplace_back("val", field.mType.mTemplateParameters.front());
  if (!field.mType.SupportsCopy(mProject)) {
    method.mArguments.back().mType.mType = PassByType::MoveReference;
    method.mBody.Add("{}[idx.GetValue()] = std::move(val);", field.mName);
  } else {
    method.mBody.Add("{}[idx.GetValue()] = val;", field.mName);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerFieldPlugin::GenerateHasElem(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Has,
                                         Annotations::MethodOption_None))
    return;
  auto method =
      ClassMethod{Naming().ContainerElemExistenceCheckerNameInCpp(*field.mField), Type{"bool"}};
  auto &arg = method.mArguments.emplace_back("elem", field.mType.mTemplateParameters.front());
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    arg.mName = "key";
  }
  arg.mType.PreventCopying();
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Has,
                                         Annotations::MethodOption_Private)) {
    method.mVisibility = Visibility::Private;
  } else if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Has,
                                                Annotations::MethodOption_Protected)) {
    method.mVisibility = Visibility::Protected;
  } else {
    method.mExposeToCSharp = true;
    method.mExposeToLua = true;
  }
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Has,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    method.mBody.Add("return {}.contains({});", field.mName, arg.mName);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

bool ContainerFieldPlugin::CanImplementDeleteElem(Class &generatedClass HOLGEN_ATTRIBUTE_UNUSED,
                                                  const ClassField &field) {
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName)) {
    return false;
  }
  auto underlyingStruct = mProject.mProject.GetStruct(field.mType.mTemplateParameters.back().mName);
  if (!underlyingStruct || !underlyingStruct->GetIdField())
    return true;
  if (!TypeInfo::Get().CppIndexedContainers.contains(field.mType.mName))
    return true;
  return false;
}

bool ContainerFieldPlugin::CanImplementSetElem(Class &generatedClass HOLGEN_ATTRIBUTE_UNUSED,
                                               const ClassField &field) {
  return TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName);
}

bool ContainerFieldPlugin::CanImplementHasElem(Class &generatedClass HOLGEN_ATTRIBUTE_UNUSED,
                                               const ClassField &field) {
  return !TypeInfo::Get().CppIndexedContainers.contains(field.mType.mName);
}
} // namespace holgen
