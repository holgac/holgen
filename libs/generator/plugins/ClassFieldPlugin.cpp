#include "ClassFieldPlugin.h"
#include "core/St.h"

namespace holgen {
  void ClassFieldPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      for (auto &fieldDefinition: cls.mStruct->mFields) {
        if (fieldDefinition.mType.mName == "Ref")
          Validate().RefField(cls, fieldDefinition);
        auto field = ClassField{
            Naming().FieldNameInCpp(fieldDefinition),
            Type{mProject.mProject, fieldDefinition.mType},
            Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue};
        field.mField = &fieldDefinition;
        if (fieldDefinition.mType.mName == "Ref" && field.mDefaultValue.empty()) {
          if (field.mType.mType == PassByType::Pointer)
            field.mDefaultValue = "nullptr";
          else
            field.mDefaultValue = "-1";
        }
        Validate().NewField(cls, field);
        cls.mFields.emplace_back(std::move(field));
      }
    }
  }
}
