#include "ClassFieldSetterPlugin.h"
#include "core/St.h"
#include "../Naming.h"

namespace holgen {
  void ClassFieldSetterPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        auto &generatedField = *generatedClass.GetField(Naming(mProject).FieldNameInCpp(fieldDefinition));
        auto &setter = generatedClass.mMethods.emplace_back(
            Naming(mProject).FieldSetterNameInCpp(fieldDefinition), Type{"void"},
            Visibility::Public, Constness::NotConst);
        auto &arg = setter.mArguments.emplace_back("val", generatedField.mType);
        if (generatedField.mType.mType == PassByType::Pointer)
          arg.mType.mType = PassByType::Pointer;
        else
          arg.mType.PreventCopying();
        setter.mBody.Add("{} = val;", generatedField.mName);
      }
    }
  }
}
