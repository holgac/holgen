#include "ClassFieldPlugin.h"
#include "core/St.h"
#include "../NamingConvention.h"

namespace holgen {
  void ClassFieldPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        auto &generatedField = generatedClass.mFields.emplace_back(
            Naming().FieldNameInCpp(fieldDefinition),
            Type{mProject.mProject, fieldDefinition.mType},
            Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue);
        generatedField.mField = &fieldDefinition;
        if (fieldDefinition.mType.mName == "Ref" && generatedField.mDefaultValue.empty()) {
          if (generatedField.mType.mType == PassByType::Pointer)
            generatedField.mDefaultValue = "nullptr";
          else
            generatedField.mDefaultValue = "-1";
        }
      }
    }
  }
}
