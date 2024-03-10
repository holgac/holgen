#include "ClassFieldSetterPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassFieldSetterPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: generatedClass.mStruct->mFields) {
        auto &generatedField = *generatedClass.GetField(
            St::GetFieldNameInCpp(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref"));

        bool isPrimitive = TypeInfo::Get().CppPrimitives.contains(generatedField.mType.mName);

        auto &setter = generatedClass.mMethods.emplace_back(
            St::GetSetterMethodName(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref"),
            Type{"void"},
            Visibility::Public,
            Constness::NotConst);
        auto &arg = setter.mArguments.emplace_back("val", generatedField.mType);
        if (!isPrimitive) {
          arg.mType.mConstness = Constness::Const;
          arg.mType.mType = PassByType::Reference;
        }
        setter.mBody.Line() << generatedField.mName << " = val;";
      }
    }
  }
}
