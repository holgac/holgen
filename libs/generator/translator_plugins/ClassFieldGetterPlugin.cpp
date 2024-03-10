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
      }
    }
  }
}
