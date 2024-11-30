#include "ContainerGetElemPlugin.h"
#include "generator/utils/CompositeIdHelper.h"
#include "core/St.h"

namespace holgen {

void ContainerGetElemPlugin::Run() {
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

void ContainerGetElemPlugin::ProcessField(Class &cls, const ClassField &field) const {
  // TODO: support getters for arrays, useful especially enum based arrays
  GenerateGetElemMethods(cls, field);
}

bool ContainerGetElemPlugin::ShouldProcess(const ClassField &field) const {
  if (!ContainerPluginBase::ShouldProcess(field))
    return false;
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                         Annotations::MethodOption_None))
    return false;
  if (TypeInfo::Get().CppSets.contains(field.mType.mName))
    return false;
  return true;
}

void ContainerGetElemPlugin::GenerateGetElemMethods(Class &cls, const ClassField &field) const {
  auto underlyingType = &field.mType.mTemplateParameters.back();
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName)) {
    underlyingType = &field.mType.mTemplateParameters.front();
  }
  auto underlyingClass = mProject.GetClass(underlyingType->mName);
  const ClassField *underlyingIdField = nullptr;
  const ClassField *compositeIdField = nullptr;
  if (underlyingClass && underlyingClass->mStruct) {
    underlyingIdField = underlyingClass->GetIdField();
    compositeIdField = CompositeIdHelper::GetObjectIdField(*underlyingClass, false);
  }
  if (compositeIdField) {
    GenerateGetElemByIdxForContainer(cls, field,
                                     Naming().ContainerElemGetterNameInCpp(*field.mField) +
                                         St::CompositeId_RawGetterSuffix,
                                     Type{underlyingIdField->mType}, Constness::Const);
    GenerateGetElemByIdxForContainer(cls, field,
                                     Naming().ContainerElemGetterNameInCpp(*field.mField) +
                                         St::CompositeId_RawGetterSuffix,
                                     Type{underlyingIdField->mType}, Constness::NotConst);
    GenerateGetElemByCompositeIdForContainer(
        cls, field, Naming().ContainerElemGetterNameInCpp(*field.mField), Constness::Const);
    GenerateGetElemByCompositeIdForContainer(
        cls, field, Naming().ContainerElemGetterNameInCpp(*field.mField), Constness::NotConst);
  } else {
    auto idxType = underlyingIdField ? underlyingIdField->mType : Type{"size_t"};
    GenerateGetElemByIdxForContainer(cls, field,
                                     Naming().ContainerElemGetterNameInCpp(*field.mField), idxType,
                                     Constness::Const);
    GenerateGetElemByIdxForContainer(cls, field,
                                     Naming().ContainerElemGetterNameInCpp(*field.mField), idxType,
                                     Constness::NotConst);
  }
}

void ContainerGetElemPlugin::ProcessMethodVisibility(const ClassField &field,
                                                     ClassMethod &method) const {
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                         Annotations::MethodOption_Private)) {
    method.mVisibility = Visibility::Private;
  } else if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                                Annotations::MethodOption_Protected)) {
    method.mVisibility = Visibility::Protected;
  } else if (method.mConstness == Constness::Const) {
    method.mExposeToCSharp = true;
    method.mExposeToLua = true;
  }
}

void ContainerGetElemPlugin::GenerateGetElemBodyForKeyedContainer(CodeBlock &codeBlock,
                                                                  const ClassField &field) const {
  codeBlock.Add("auto it = {}.find(idx);", field.mName);
  codeBlock.Add("if (it == {}.end())", field.mName);
  codeBlock.Indent(1);
  codeBlock.Line() << "return nullptr;";
  codeBlock.Indent(-1);
  codeBlock.Add("return &it->second;");
}

void ContainerGetElemPlugin::GenerateBoundsCheckForIndexedContainer(
    CodeBlock &codeBlock, const ClassField &field, const std::string &idxExpression) const {
  codeBlock.Add("if (size_t({}) >= {}.size())", idxExpression, field.mName);
  codeBlock.Indent(1);
  codeBlock.Line() << "return nullptr;";
  codeBlock.Indent(-1);
}

void ContainerGetElemPlugin::GenerateGetElemBodyForCompositeId(
    CodeBlock &codeBlock, const ClassField &field, const std::string &idxExpression) const {
  auto underlyingType = &field.mType.mTemplateParameters.back();
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName)) {
    underlyingType = &field.mType.mTemplateParameters.front();
  }
  auto underlyingClass = mProject.GetClass(underlyingType->mName);
  // TODO: @container(unsafe) attribute that avoids bounds checks, can return ref instead of ptr
  GenerateBoundsCheckForIndexedContainer(codeBlock, field, idxExpression);
  codeBlock.Add("auto& elem = {}[{}];", field.mName, idxExpression);
  auto compositeIdType = CompositeIdHelper::GetCompositeIdType(mProject, *underlyingClass);
  codeBlock.Add("if (elem.{}() != id.{}()) {{",
                Naming().FieldGetterNameInCpp(
                    CompositeIdHelper::GetObjectVersionField(*underlyingClass)->mField->mName),
                Naming().FieldGetterNameInCpp(
                    CompositeIdHelper::GetIdVersionField(*compositeIdType)->mField->mName));
  codeBlock.Indent(1);
  codeBlock.Add("return nullptr;");
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  if (field.mType.mTemplateParameters.back().mType == PassByType::Pointer) {
    codeBlock.Add("return elem;");
  } else {
    codeBlock.Add("return &elem;");
  }
}

void ContainerGetElemPlugin::GenerateGetElemBodyForIndexedContainer(
    CodeBlock &codeBlock, const ClassField &field, const std::string &idxExpression) const {
  // TODO: @container(unsafe) attribute that avoids bounds checks, can return ref instead of ptr
  GenerateBoundsCheckForIndexedContainer(codeBlock, field, idxExpression);
  if (field.mType.mTemplateParameters.back().mType == PassByType::Pointer) {
    codeBlock.Add("return {}[{}];", field.mName, idxExpression);
  } else {
    codeBlock.Add("return &{}[{}];", field.mName, idxExpression);
  }
}

void ContainerGetElemPlugin::GenerateGetElemByIdxForContainer(Class &cls, const ClassField &field,
                                                              const std::string &methodName,
                                                              const Type &idxType,
                                                              Constness constness) const {
  auto underlyingType = &field.mType.mTemplateParameters.back();
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName)) {
    underlyingType = &field.mType.mTemplateParameters.front();
  }
  auto method = ClassMethod{methodName, *underlyingType, Visibility::Public, constness};
  method.mReturnType.mType = PassByType::Pointer;
  method.mReturnType.mConstness = constness;
  method.mArguments.emplace_back("idx", idxType);
  ProcessMethodVisibility(field, method);
  auto fixedSizeEnumArray = mProject.mProject.GetEnum(field.mField->mType.mArraySize);
  std::string idxExpression = "idx";
  if (field.mField->GetMatchingAttribute(Annotations::Container, Annotations::Container_Get,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
  } else if (TypeInfo::Get().CppKeyedContainers.contains(field.mType.mName)) {
    GenerateGetElemBodyForKeyedContainer(method.mBody, field);
  } else if (fixedSizeEnumArray) {
    method.mArguments.front().mType =
        Type{fixedSizeEnumArray->mName, PassByType::Reference, Constness::Const};
    method.mReturnType.mType = PassByType::Reference;
    method.mBody.Add("return {}[{}.GetValue()];", field.mName, idxExpression);
  } else {
    GenerateGetElemBodyForIndexedContainer(method.mBody, field, idxExpression);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ContainerGetElemPlugin::GenerateGetElemByCompositeIdForContainer(Class &cls,
                                                                      const ClassField &field,
                                                                      const std::string &methodName,
                                                                      Constness constness) const {
  auto underlyingType = &field.mType.mTemplateParameters.back();
  if (TypeInfo::Get().CppFixedSizeContainers.contains(field.mType.mName)) {
    underlyingType = &field.mType.mTemplateParameters.front();
  }
  auto underlyingClass = mProject.GetClass(underlyingType->mName);

  auto method = ClassMethod{methodName, *underlyingType, Visibility::Public, constness};
  method.mReturnType.mType = PassByType::Pointer;
  method.mReturnType.mConstness = constness;
  ProcessMethodVisibility(field, method);

  auto compositeIdType = CompositeIdHelper::GetCompositeIdType(mProject, *underlyingClass);
  method.mArguments.emplace_back(
      "id", Type{compositeIdType->mName, PassByType::Reference, Constness::Const});
  auto idxExpression =
      std::format("id.{}()",
                  Naming().FieldGetterNameInCpp(
                      CompositeIdHelper::GetIdIdField(*compositeIdType)->mField->mName));
  GenerateGetElemBodyForCompositeId(method.mBody, field, idxExpression);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
