#include "ClassFieldGetterPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void ClassFieldGetterPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    for (auto &field: cls.mFields) {
      if (!field.mField)
        continue;
      if (field.mField->mType.mName == St::Variant)
        continue;
      if (cls.GetMethod(Naming().FieldSetterNameInCpp(*field.mField), Constness::Const) ||
          cls.GetMethod(Naming().FieldSetterNameInCpp(*field.mField), Constness::NotConst))
        continue;
      ProcessField(cls, field, true);
      ProcessField(cls, field, false);

      if (field.mField->mType.mName == "Ref")
        ProcessRefField(cls, field);
    }
  }
}

void ClassFieldGetterPlugin::ProcessRefField(Class &cls, ClassField &field) const {
  auto underlyingType =
      mProject.mProject.GetStruct(field.mField->mType.mTemplateParameters.back().mName);
  if (!underlyingType->GetAnnotation(Annotations::Managed))
    return;
  for (int i = 0; i < 2; ++i) {
    Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
    auto method = ClassMethod{Naming().FieldGetterNameInCpp(*field.mField, true),
                              Type{mProject, field.mField->mDefinitionSource,
                                   field.mField->mType.mTemplateParameters[0], PassByType::Pointer,
                                   constness},
                              Visibility::Public, constness};
    method.mBody.Add("return {}::{}({});", underlyingType->mName, St::ManagedObject_Getter,
                     field.mName);
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void ClassFieldGetterPlugin::ProcessField(Class &cls, ClassField &field, bool isConst) const {
  // non-const getter for non-primitives only
  if (!isConst && TypeInfo::Get().CppPrimitives.contains(field.mType.mName))
    return;
  if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Get,
                                         Annotations::MethodOption_None))
    return;
  auto constness = isConst ? Constness::Const : Constness::NotConst;
  auto method = ClassMethod{Naming().FieldGetterNameInCpp(*field.mField), field.mType,
                            Visibility::Public, constness};
  method.mReturnType.PreventCopying(isConst);
  if (field.mType.mType == PassByType::Pointer)
    method.mReturnType.mType = PassByType::Pointer;

  if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Get,
                                         Annotations::MethodOption_Custom)) {
    method.mUserDefined = true;
    if (field.mField->mType.mName == St::UserData) {
      method.mReturnType.mName = "void";
      method.mReturnType.mType = PassByType::Pointer;
    }
  } else {
    if (field.mField->mType.mName == St::UserData) {
      method.mTemplateParameters.emplace_back("typename", "T");
      method.mReturnType.mName = "T";
      method.mBody.Add("return reinterpret_cast<{}T*>({});", isConst ? "const " : "", field.mName);
    } else {
      method.mBody.Add("return {};", field.mName);
    }
  }
  if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Get,
                                         Annotations::MethodOption_Private)) {
    method.mVisibility = Visibility::Private;
  } else if (field.mField->GetMatchingAttribute(Annotations::Field, Annotations::Field_Get,
                                                Annotations::MethodOption_Protected)) {
    method.mVisibility = Visibility::Protected;
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
