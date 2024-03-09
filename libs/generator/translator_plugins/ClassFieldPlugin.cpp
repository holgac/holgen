#include "ClassFieldPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassFieldPlugin::ProcessRefField(Class &generatedClass, ClassField &generatedField,
                                   const FieldDefinition &fieldDefinition) const {
    if (!fieldDefinition.mDefaultValue.empty())
      generatedField.mDefaultValue = fieldDefinition.mDefaultValue;
    else
      generatedField.mDefaultValue = "-1";
    auto refStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters[0].mName);
    auto refStructId = refStruct->GetIdField();
    TypeInfo::Get().ConvertToType(generatedField.mType, refStructId->mType);
    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &getter = generatedClass.mMethods.emplace_back();
      getter.mName = St::GetGetterMethodName(fieldDefinition.mName);
      getter.mBody.Add("return {}::{}({});", refStruct->mName, St::ManagedObject_Getter,
                       generatedField.mName);

      TypeInfo::Get().ConvertToType(getter.mReturnType, fieldDefinition.mType.mTemplateParameters[0]);
      getter.mConstness = constness;
      getter.mReturnType.mConstness = constness;
      getter.mReturnType.mType = PassByType::Pointer;
    }
  }

  void ClassFieldPlugin::EnrichClasses() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {

        auto &generatedField = generatedClass.mFields.emplace_back();
        generatedField.mField = &fieldDefinition;

        bool isRef = fieldDefinition.mType.mName == "Ref";
        generatedField.mName = St::GetFieldNameInCpp(fieldDefinition.mName, isRef);
        if (isRef) {
          ProcessRefField(generatedClass, generatedField, fieldDefinition);
        } else {
          generatedField.mDefaultValue = fieldDefinition.mDefaultValue;
          TypeInfo::Get().ConvertToType(generatedField.mType, fieldDefinition.mType);
        }

      }
    }
  }
}
