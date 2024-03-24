#include "ClassFieldPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassFieldPlugin::ProcessRefField(Class &generatedClass, ClassField &generatedField,
                                         const FieldDefinition &fieldDefinition) const {
    if (generatedField.mDefaultValue.empty())
      generatedField.mDefaultValue = "-1";
    auto refStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters.back().mName);
    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &getter = generatedClass.mMethods.emplace_back(
          St::GetGetterMethodName(fieldDefinition.mName),
          Type{mProject.mProject, fieldDefinition.mType.mTemplateParameters[0], PassByType::Pointer, constness},
          Visibility::Public,
          constness);
      getter.mBody.Add("return {}::{}({});", refStruct->mName, St::ManagedObject_Getter,
                       generatedField.mName);
      if (i == 0)
        getter.mExposeToLua = true;
    }
  }

  void ClassFieldPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        bool isRef = fieldDefinition.mType.mName == "Ref";

        auto &generatedField = generatedClass.mFields.emplace_back(
            St::GetFieldNameInCpp(fieldDefinition.mName, isRef), Type{mProject.mProject, fieldDefinition.mType},
            Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue);
        generatedField.mField = &fieldDefinition;
        if (isRef) {
          // TODO: separate ref plugin, or move to getter
          ProcessRefField(generatedClass, generatedField, fieldDefinition);
        }
      }
    }
  }
}
