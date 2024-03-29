#include "ClassFieldGetterPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"
#include "../NamingConvention.h"

namespace holgen {
  void ClassFieldGetterPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      for (auto &field: cls.mFields) {
        if (!field.mField)
          continue;
        ProcessField(cls, field, true);
        ProcessField(cls, field, false);

        if (field.mField->mType.mName == "Ref")
          ProcessRefField(cls, field);
      }
    }
  }

  void ClassFieldGetterPlugin::ProcessRefField(Class &cls, ClassField &field) const {
    auto refStruct = mProject.mProject.GetStruct(field.mField->mType.mTemplateParameters.back().mName);
    if (refStruct->GetAnnotation(Annotations::Managed) == nullptr)
      return;
    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &getter = cls.mMethods.emplace_back(
          Naming().FieldGetterNameInCpp(*field.mField, true),
          Type{mProject.mProject, field.mField->mType.mTemplateParameters[0], PassByType::Pointer, constness},
          Visibility::Public,
          constness);
      getter.mBody.Add("return {}::{}({});", refStruct->mName, St::ManagedObject_Getter,
                       field.mName);
      if (i == 0)
        getter.mExposeToLua = true;
    }
  }

  void ClassFieldGetterPlugin::ProcessField(Class &cls, ClassField &field, bool isConst) const {
    // non-const getter for non-primitives only
    if (!isConst && TypeInfo::Get().CppPrimitives.contains(field.mType.mName))
      return;
    auto constness = isConst ? Constness::Const : Constness::NotConst;
    auto &getter = cls.mMethods.emplace_back(
        Naming().FieldGetterNameInCpp(*field.mField), field.mType,
        Visibility::Public, constness);
    getter.mReturnType.PreventCopying(isConst);
    if (field.mType.mType == PassByType::Pointer)
      getter.mReturnType.mType = PassByType::Pointer;
    getter.mBody.Add("return {};", field.mName);
  }
}