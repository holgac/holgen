#include "ClassFieldPlugin.h"
#include "core/St.h"

namespace holgen {
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
      }
    }
  }
}
