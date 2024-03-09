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

        bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);
        {
          auto &getter = generatedClass.mMethods.emplace_back();
          getter.mName = St::GetGetterMethodName(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref");
          getter.mBody.Line() << "return " << generatedField.mName << ";";
          getter.mReturnType = generatedField.mType;
          getter.mConstness = Constness::Const;
          if (!isPrimitive) {
            getter.mReturnType.mConstness = Constness::Const;
            getter.mReturnType.mType = PassByType::Reference;
          }
        }
        // non-const getter for non-primitives only
        if (!isPrimitive) {
          auto &getter = generatedClass.mMethods.emplace_back();
          getter.mName = St::GetGetterMethodName(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref");
          getter.mBody.Line() << "return " << generatedField.mName << ";";
          getter.mReturnType = generatedField.mType;
          getter.mConstness = Constness::NotConst;
          getter.mReturnType.mType = PassByType::Reference;
        }
      }
    }
  }
}
