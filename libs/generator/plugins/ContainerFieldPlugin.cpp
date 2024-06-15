#include "ContainerFieldPlugin.h"
#include "../NamingConvention.h"
#include "core/Annotations.h"
#include "core/St.h"

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
  for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
    ProcessIndex(cls, field, annotation);
  }
  GenerateNextIndexField(cls, field);
  // TODO: support getters for arrays, useful especially enum based arrays
  GenerateAddElem(cls, field, true);
  GenerateAddElem(cls, field, false);
  GenerateGetElem(cls, field);
  if (CanImplementHasElem(cls, field))
    GenerateHasElem(cls, field);
  if (CanImplementDeleteElem(cls, field))
    GenerateDeleteElem(cls, field);
  GenerateGetCount(cls, field);
}

void ContainerFieldPlugin::ProcessIndex(Class &cls, const ClassField &field,
                                        const AnnotationDefinition &annotationDefinition) {
  Validate().IndexAnnotation(cls, field, annotationDefinition);
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

  for (int i = 0; i < 2; ++i) {
    Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
    auto method =
        ClassMethod{Naming().ContainerIndexGetterNameInCpp(*field.mField, annotationDefinition),
                    Type{mProject, field.mField->mDefinitionSource, underlyingType,
                         PassByType::Pointer, constness},
                    Visibility::Public, constness};
    if (i == 0)
      method.mExposeToLua = true;
    method.mArguments.emplace_back(
        "key",
        Type{mProject, fieldIndexedOn.mField->mDefinitionSource, fieldIndexedOn.mField->mType});
    method.mArguments.back().mType.PreventCopying();

    method.mBody.Add("auto it = {}.find(key);", indexField.mName);
    method.mBody.Add("if (it == {}.end())", indexField.mName);
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
  Validate().NewField(cls, indexField);
  cls.mFields.push_back(std::move(indexField));
}

void ContainerFieldPlugin::GenerateAddElem(Class &cls, const ClassField &field, bool useMoveRef) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                         Annotations::MethodOption_None))
    return;
  auto &underlyingType = field.mType.mTemplateParameters.back();
  if (useMoveRef && TypeInfo::Get().CppPrimitives.contains(underlyingType.mName))
    return;
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  const ClassField *underlyingIdField = nullptr;
  if (underlyingClass && underlyingClass->mStruct) {
    if (!useMoveRef &&
        underlyingClass->mStruct->GetMatchingAttribute(Annotations::Struct,
                                                       Annotations::Struct_NonCopyable))
      return;
    underlyingIdField = underlyingClass->GetIdField();
  }
  bool isKeyedContainer = TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName);

  auto method = ClassMethod{Naming().ContainerElemAdderNameInCpp(*field.mField),
                            Type{underlyingType.mName, PassByType::Pointer}, Visibility::Public,
                            Constness::NotConst};
  method.mArguments.emplace_back("elem", underlyingType);
  if (useMoveRef) {
    method.mArguments.back().mType.mType = PassByType::MoveReference;
  } else {
    method.mArguments.back().mType.PreventCopying();
  }
  if (underlyingIdField) {
    method.mArguments.back().mType.mConstness = Constness::NotConst;
  }

  // TODO: change to useMoveRef after implementing ReadFromLua
  if (!useMoveRef)
    method.mExposeToLua = true;

  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                           Annotations::MethodOption_Private)) {
      method.mVisibility = Visibility::Private;
    } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                  Annotations::Container_Add,
                                                  Annotations::MethodOption_Protected)) {
      method.mVisibility = Visibility::Protected;
    }
    CodeBlock validators;
    CodeBlock inserters;
    for (const auto &annotation: field.mField->GetAnnotations(Annotations::Index)) {
      auto indexOn = annotation.GetAttribute(Annotations::Index_On);
      auto &fieldIndexedOn = *underlyingClass->GetFieldFromDefinitionName(indexOn->mValue.mName);
      auto indexFieldName = Naming().FieldIndexNameInCpp(*field.mField, annotation);
      auto getterMethodName = Naming().FieldGetterNameInCpp(*fieldIndexedOn.mField);
      validators.Add("if ({}.contains(elem.{}())) {{", indexFieldName, getterMethodName);
      validators.Indent(1);
      validators.Add(R"(HOLGEN_WARN("{} with {}={{}} already exists", elem.{}());)",
                     underlyingClass->mName, indexOn->mValue.mName, getterMethodName);
      validators.Add("return nullptr;");
      validators.Indent(-1);
      validators.Add("}}");
      inserters.Add("{}.emplace(elem.{}(), newId);", indexFieldName, getterMethodName);
    }
    method.mBody.Add(std::move(validators));

    if (isKeyedContainer) {
      method.mBody.Add("auto newId = {}NextId;", field.mName);
      method.mBody.Add("++{}NextId;", field.mName);
    } else if (!inserters.mContents.empty() || underlyingIdField) {
      method.mBody.Add("auto newId = {}.size();", field.mName);
    }
    method.mBody.Add(std::move(inserters));

    if (underlyingIdField) {
      method.mBody.Add("elem.{}(newId);",
                       Naming().FieldSetterNameInCpp(*underlyingIdField->mField));
    }

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
    } else {
      method.mBody.Add("return &({}.emplace_back({}));", field.mName, elemToInsert);
    }
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerFieldPlugin::GenerateGetElem(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                         Annotations::MethodOption_None))
    return;

  auto &underlyingType = field.mType.mTemplateParameters.back();
  auto underlyingClass = mProject.GetClass(underlyingType.mName);
  const ClassField *underlyingIdField = nullptr;
  if (underlyingClass && underlyingClass->mStruct)
    underlyingIdField = underlyingClass->GetIdField();
  if (TypeInfo::Get().CppSets.contains(field.mType.mName))
    return;
  bool isKeyedContainer = TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName);
  for (int i = 0; i < 2; ++i) {
    auto constness = i == 0 ? Constness::Const : Constness::NotConst;
    auto method = ClassMethod{Naming().ContainerElemGetterNameInCpp(*field.mField), underlyingType,
                              Visibility::Public, constness};
    method.mReturnType.mType = PassByType::Pointer;
    method.mReturnType.mConstness = constness;
    if (i == 0)
      method.mExposeToLua = true;
    bool isSigned = false;
    if (underlyingIdField) {
      auto &arg = method.mArguments.emplace_back("idx",
                                                 Type{mProject,
                                                      underlyingIdField->mField->mDefinitionSource,
                                                      underlyingIdField->mField->mType});
      if (TypeInfo::Get().SignedIntegralTypes.contains(arg.mType.mName))
        isSigned = true;
    } else {
      method.mArguments.emplace_back("idx", Type{"size_t"});
    }

    if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                           Annotations::MethodOption_Custom)) {
      method.mUserDefined = true;
    } else {
      if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                             Annotations::MethodOption_Private)) {
        method.mVisibility = Visibility::Private;
      } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                    Annotations::Container_Get,
                                                    Annotations::MethodOption_Protected)) {
        method.mVisibility = Visibility::Protected;
      }
      // TODO: @container(unsafe) attribute that avoids bounds checks, can return ref instead of ptr
      if (isKeyedContainer) {
        method.mBody.Add("auto it = {}.find(idx);", field.mName);
        method.mBody.Add("if (it == {}.end())", field.mName);
      } else {
        auto line = method.mBody.Line();
        line << "if (idx >= " << field.mName << ".size()";
        if (isSigned) {
          line << " || idx < 0";
        }
        line << ")";
      }
      method.mBody.Indent(1);
      method.mBody.Line() << "return nullptr;";
      method.mBody.Indent(-1);
      if (isKeyedContainer) {
        method.mBody.Add("return &it->second;");
      } else {
        method.mBody.Line() << "return &" << field.mName << "[idx];";
      }
    }
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void ContainerFieldPlugin::GenerateGetCount(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Count,
                                         Annotations::MethodOption_None))
    return;
  auto method = ClassMethod{Naming().ContainerElemCountNameInCpp(*field.mField), Type{"size_t"}};
  method.mExposeToLua = true;

  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Count,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Count,
                                           Annotations::MethodOption_Private)) {
      method.mVisibility = Visibility::Private;
    } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                  Annotations::Container_Count,
                                                  Annotations::MethodOption_Protected)) {
      method.mVisibility = Visibility::Protected;
    }
    method.mBody.Add("return {}.size();", field.mName);
  }

  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerFieldPlugin::GenerateDeleteElem(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_None))
    return;

  auto method = ClassMethod{Naming().ContainerElemDeleterNameInCpp(*field.mField), Type{"void"},
                            Visibility::Public, Constness::NotConst};
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    method.mArguments.emplace_back("key", field.mType.mTemplateParameters.front());
  } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
    method.mArguments.emplace_back("elem", field.mType.mTemplateParameters.front());
  } else {
    method.mArguments.emplace_back("idx", Type{"size_t"});
  }

  method.mExposeToLua = true;
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Delete,
                                           Annotations::MethodOption_Private)) {
      method.mVisibility = Visibility::Private;
    } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                  Annotations::Container_Delete,
                                                  Annotations::MethodOption_Protected)) {
      method.mVisibility = Visibility::Protected;
    }
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
      method.mBody.Add("auto ptr = {}({});", Naming().ContainerElemGetterNameInCpp(*field.mField),
                       method.mArguments.back().mName);
      method.mBody.Add(std::move(indexDeleters));
    }

    if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
      method.mBody.Add("{}.erase(key);", field.mName);
    } else if (TypeInfo::Get().CppSets.contains(field.mType.mName)) {
      method.mBody.Add("{}.erase(elem);", field.mName);
    } else {
      method.mBody.Add("if (idx != {}.size() - 1) {{", field.mName);
      method.mBody.Indent(1);
      method.mBody.Add(std::move(indexReassigners));
      method.mBody.Add("{0}[idx] = std::move({0}.back());", field.mName);
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
      method.mBody.Add("{}.pop_back();", field.mName);
    }
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
  method.mExposeToLua = true;
  auto &arg = method.mArguments.emplace_back("elem", field.mType.mTemplateParameters.front());
  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    arg.mName = "key";
  }
  arg.mType.PreventCopying();
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Has,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else {
    if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Has,
                                           Annotations::MethodOption_Private)) {
      method.mVisibility = Visibility::Private;
    } else if (field.mField->GetMatchingAttribute(Annotations::Container,
                                                  Annotations::Container_Has,
                                                  Annotations::MethodOption_Protected)) {
      method.mVisibility = Visibility::Protected;
    }
    method.mBody.Add("return {}.contains({});", field.mName, arg.mName);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

bool ContainerFieldPlugin::CanImplementDeleteElem(Class &generatedClass HOLGEN_ATTRIBUTE_UNUSED,
                                                  const ClassField &field) {
  auto underlyingStruct = mProject.mProject.GetStruct(field.mType.mTemplateParameters.back().mName);
  if (!underlyingStruct || !underlyingStruct->GetIdField())
    return true;
  if (!TypeInfo::Get().CppIndexedContainers.contains(field.mType.mName))
    return true;
  return false;
}

bool ContainerFieldPlugin::CanImplementHasElem(Class &generatedClass HOLGEN_ATTRIBUTE_UNUSED,
                                               const ClassField &field) {
  return !TypeInfo::Get().CppIndexedContainers.contains(field.mType.mName);
}

void ContainerFieldPlugin::GenerateNextIndexField(Class &cls, const ClassField &field) {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Add,
                                         Annotations::MethodOption_None))
    return;

  if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    auto underlyingIdField =
        mProject.GetClass(field.mType.mTemplateParameters.back().mName)->GetIdField();
    auto nextIdField = ClassField{field.mName + "NextId",
                                  Type{mProject, underlyingIdField->mField->mDefinitionSource,
                                       underlyingIdField->mField->mType}};
    nextIdField.mDefaultValue = "0";
    Validate().NewField(cls, nextIdField);
    cls.mFields.push_back(std::move(nextIdField));
  }
}

} // namespace holgen
