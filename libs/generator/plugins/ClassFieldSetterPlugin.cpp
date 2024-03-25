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
        bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);

        auto &setter = generatedClass.mMethods.emplace_back(
            Naming(mProject).FieldSetterNameInCpp(fieldDefinition),
            Type{"void"},
            Visibility::Public,
            Constness::NotConst);
        auto &arg = setter.mArguments.emplace_back("val", generatedField.mType);
        if (generatedField.mType.mType == PassByType::Pointer) {
          arg.mType.mType = PassByType::Pointer;
        } else if (!isPrimitive) {
          arg.mType.mConstness = Constness::Const;
          arg.mType.mType = PassByType::Reference;
        }
        setter.mBody.Line() << generatedField.mName << " = val;";
      }
    }
  }
}
