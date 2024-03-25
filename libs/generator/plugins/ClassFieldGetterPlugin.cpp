#include "ClassFieldGetterPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"
#include "../Naming.h"

namespace holgen {
  void ClassFieldGetterPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
        bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);
        PassByType getterReturnPassByType = PassByType::Value;
        if (generatedField.mType.mType == PassByType::Pointer)
          getterReturnPassByType = PassByType::Pointer;
        else if (!isPrimitive)
          getterReturnPassByType = PassByType::Reference;

        {
          auto &getter = generatedClass.mMethods.emplace_back(
              Naming(mProject).FieldGetterNameInCpp(fieldDefinition),
              generatedField.mType,
              Visibility::Public, Constness::Const
          );
          if (!isPrimitive)
            getter.mReturnType.mConstness = Constness::Const;
          getter.mReturnType.mType = getterReturnPassByType;
          getter.mBody.Line() << "return " << generatedField.mName << ";";
        }
        // non-const getter for non-primitives only
        if (!isPrimitive) {
          auto &getter = generatedClass.mMethods.emplace_back(
              Naming(mProject).FieldGetterNameInCpp(fieldDefinition),
              generatedField.mType,
              Visibility::Public, Constness::NotConst
          );
          getter.mReturnType.mType = getterReturnPassByType;
          getter.mBody.Line() << "return " << generatedField.mName << ";";
        }

        if (fieldDefinition.mType.mName == "Ref")
          ProcessRefField(generatedClass, generatedField, fieldDefinition);
      }
    }
  }

  void ClassFieldGetterPlugin::ProcessRefField(Class &generatedClass, ClassField &generatedField,
                                               const FieldDefinition &fieldDefinition) const {
    auto refStruct = mProject.mProject.GetStruct(fieldDefinition.mType.mTemplateParameters.back().mName);
    if (refStruct->GetAnnotation(Annotations::Managed) == nullptr)
      return;
    for (int i = 0; i < 2; ++i) {
      Constness constness = i == 0 ? Constness::Const : Constness::NotConst;
      auto &getter = generatedClass.mMethods.emplace_back(
          Naming(mProject).FieldGetterNameInCpp(fieldDefinition, true),
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