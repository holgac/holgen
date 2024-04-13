#include "ClassFieldPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
  void ClassFieldPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      ProcessStructDefinition(cls, *cls.mStruct);
    }
  }

  void ClassFieldPlugin::ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition) {
    for(auto& mixin: structDefinition.mMixins) {
      ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
    }
    for (auto &fieldDefinition: structDefinition.mFields) {
      // Skip ids; they're processed in their own plugin
      if (fieldDefinition.GetAnnotation(Annotations::Id))
        continue;
      // TODO: if @optimize(alignment) (or @packed?), order the fields to minimize padding. Default to it?
      if (fieldDefinition.mType.mName == "Ref")
        Validate().RefField(cls, fieldDefinition);
      auto field = ClassField{
          Naming().FieldNameInCpp(fieldDefinition),
          Type{mProject, fieldDefinition.mType},
          Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue};
      field.mField = &fieldDefinition;
      if (fieldDefinition.mType.mName == St::UserData) {
        field.mType = Type{"void", PassByType::Pointer};
        field.mDefaultValue = "nullptr";
      } else if (fieldDefinition.mType.mName == "Ref" && field.mDefaultValue.empty()) {
        if (field.mType.mType == PassByType::Pointer)
          field.mDefaultValue = "nullptr";
        else
          field.mDefaultValue = "-1";
      }
      FillComments(fieldDefinition, field.mComments);
      Validate().NewField(cls, field);
      cls.mFields.push_back(std::move(field));
    }
  }
}
