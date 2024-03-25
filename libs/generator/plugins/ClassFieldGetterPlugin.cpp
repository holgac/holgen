#include "ClassFieldGetterPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassFieldGetterPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        auto &generatedField = *generatedClass.GetField(
            St::GetFieldNameInCpp(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref"));
        bool isRef = fieldDefinition.mType.mName == "Ref";

        bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);
        {
          auto &getter = generatedClass.mMethods.emplace_back(
              St::GetGetterMethodName(fieldDefinition.mName, isRef),
              generatedField.mType,
              Visibility::Public, Constness::Const
          );
          if (!isPrimitive) {
            getter.mReturnType.mConstness = Constness::Const;
            getter.mReturnType.mType = PassByType::Reference;
          }
          getter.mBody.Line() << "return " << generatedField.mName << ";";
        }
        // non-const getter for non-primitives only
        if (!isPrimitive) {
          auto &getter = generatedClass.mMethods.emplace_back(
              St::GetGetterMethodName(fieldDefinition.mName, isRef),
              generatedField.mType,
              Visibility::Public, Constness::NotConst
          );
          getter.mReturnType.mType = PassByType::Reference;
          getter.mBody.Line() << "return " << generatedField.mName << ";";
        }

        if (isRef)
          ProcessRefField(generatedClass, generatedField, fieldDefinition);
      }
    }
  }

  void ClassFieldGetterPlugin::ProcessRefField(Class &generatedClass, ClassField &generatedField,
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
}