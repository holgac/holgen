#include "ClassFieldPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void ClassFieldPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    ProcessStructDefinition(cls, *cls.mStruct);
  }
}

void ClassFieldPlugin::ProcessStructDefinition(Class &cls,
                                               const StructDefinition &structDefinition) {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
  }
  for (auto &fieldDefinition: structDefinition.mFields) {
    // Skip ids; they're processed in their own plugin
    if (fieldDefinition.GetAnnotation(Annotations::Id))
      continue;
    // Skip special types; they're processed in their own plugins
    if (fieldDefinition.mType.mName == St::Variant ||
        fieldDefinition.mType.mName == St::Lua_CustomData)
      continue;
    // TODO: if @optimize(alignment) (or @packed?), order the fields to minimize padding. Default to
    // it? ordering should probably be in code generation rather than translator plugins
    if (fieldDefinition.mType.mName == "Ref")
      Validate().RefField(cls, fieldDefinition);
    auto field =
        ClassField{Naming().FieldNameInCpp(fieldDefinition),
                   Type{mProject, fieldDefinition.mDefinitionSource, fieldDefinition.mType},
                   Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue};
    field.mField = &fieldDefinition;
    if (field.mType.mType == PassByType::Pointer &&
        field.mField->GetMatchingAnnotation(Annotations::Field, Annotations::Field_ConstPtr)) {
      field.mType.mConstness = Constness::Const;
    }
    if (fieldDefinition.mType.mName == St::UserData) {
      field.mType = Type{"void", PassByType::Pointer};
      field.mDefaultValue = "nullptr";
    } else if (fieldDefinition.mType.mName == "Ref" && !field.mDefaultValue.has_value()) {
      if (field.mType.mType == PassByType::Pointer)
        field.mDefaultValue = "nullptr";
      else
        field.mDefaultValue = "-1";
    } else if (field.mType.mType == PassByType::Pointer) {
      field.mDefaultValue = "nullptr";
    }
    FillComments(fieldDefinition, field.mComments);
    Validate().NewField(cls, field);
    cls.mFields.push_back(std::move(field));
  }
}
} // namespace holgen
