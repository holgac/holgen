#include "CompositeIdTypePlugin.h"

#include "generator/utils/CompositeIdHelper.h"
#include "core/St.h"

namespace holgen {
void CompositeIdTypePlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    Process(cls);
  }
}

bool CompositeIdTypePlugin::ShouldProcess(const Class &cls) const {
  if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::CompositeIdType))
    return false;
  return true;
}

void CompositeIdTypePlugin::Process(Class &cls) const {
  AddConstructor(cls);
  AddIsValidMethod(cls);
  AddLessThanOperator(cls);
}

void CompositeIdTypePlugin::AddConstructor(Class &cls) const {
  // only add if the user hasn't defined any.
  if (!cls.mConstructors.empty())
    return;
  auto ctor = ClassConstructor{};
  std::vector<const ClassField *> fields;
  auto typeField = CompositeIdHelper::GetIdTypeField(cls);
  auto idField = CompositeIdHelper::GetIdIdField(cls);
  auto versionField = CompositeIdHelper::GetIdVersionField(cls);
  ctor.mArguments.emplace_back(typeField->mField->mName, typeField->mType);
  ctor.mArguments.emplace_back(idField->mField->mName, idField->mType);
  ctor.mArguments.emplace_back(versionField->mField->mName, versionField->mType);

  ctor.mInitializerList.emplace_back(typeField->mName, typeField->mField->mName);
  ctor.mInitializerList.emplace_back(versionField->mName, versionField->mField->mName);
  ctor.mInitializerList.emplace_back(idField->mName, idField->mField->mName);

  cls.mConstructors.push_back(std::move(ctor));
}

void CompositeIdTypePlugin::AddIsValidMethod(Class &cls) const {
  auto existingFunc = cls.GetMethod(St::CompositeId_IsValid, Constness::Const);
  if (existingFunc)
    return;
  auto method = ClassMethod{St::CompositeId_IsValid, Type{"bool"}};
  auto typeField = CompositeIdHelper::GetIdTypeField(cls);
  method.mBody.Add("return {} != {}::Invalid;", typeField->mName, typeField->mType.mName);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void CompositeIdTypePlugin::AddLessThanOperator(Class &cls) const {
  auto existingFunc = cls.GetMethod("operator<", Constness::Const);
  if (existingFunc)
    return;
  auto method = ClassMethod{"operator<", Type{"bool"}};
  method.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
  std::vector<const ClassField *> fields;
  fields.push_back(CompositeIdHelper::GetIdTypeField(cls));
  fields.push_back(CompositeIdHelper::GetIdIdField(cls));
  fields.push_back(CompositeIdHelper::GetIdVersionField(cls));
  for (auto &field: fields) {
    method.mBody.Add("if ({0} != rhs.{0})", field->mName);
    method.mBody.Indent(1);
    method.mBody.Add("return {0} < rhs.{0};", field->mName);
    method.mBody.Indent(-1);
  }
  method.mBody.Add("return false;");


  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
