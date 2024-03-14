#include "ClassFieldPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassFieldPlugin::ProcessRefField(Class &generatedClass, ClassField &generatedField,
                                         const FieldDefinition &fieldDefinition) const {
    if (generatedField.mDefaultValue.empty())
      generatedField.mDefaultValue = "-1";
    auto refStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters.back().mName);
    auto refStructId = refStruct->GetIdField();
    generatedField.mType = Type{refStructId->mType};
    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &getter = generatedClass.mMethods.emplace_back(
          St::GetGetterMethodName(fieldDefinition.mName),
          Type{fieldDefinition.mType.mTemplateParameters[0], PassByType::Pointer, constness},
          Visibility::Public,
          constness);
      getter.mBody.Add("return {}::{}({});", refStruct->mName, St::ManagedObject_Getter,
                       generatedField.mName);
    }
  }

  void ClassFieldPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        bool isRef = fieldDefinition.mType.mName == "Ref";

        auto &generatedField = generatedClass.mFields.emplace_back(
            St::GetFieldNameInCpp(fieldDefinition.mName, isRef), Type{fieldDefinition.mType},
            Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue);
        generatedField.mField = &fieldDefinition;
        if (isRef) {
          // TODO: separate ref plugin
          ProcessRefField(generatedClass, generatedField, fieldDefinition);
        }
      }
    }
  }
}
