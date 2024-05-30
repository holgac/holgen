#include "ClassIdFieldPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
void ClassIdFieldPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    ProcessStructDefinition(cls, *cls.mStruct);
  }
}

void ClassIdFieldPlugin::ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition) {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
  }
  for (auto &fieldDefinition: structDefinition.mFields) {
    if (!fieldDefinition.GetAnnotation(Annotations::Id))
      continue;
    auto field = ClassField{
        Naming().FieldNameInCpp(fieldDefinition),
        Type{mProject, fieldDefinition.mType},
        Visibility::Private, Staticness::NotStatic, fieldDefinition.mDefaultValue};
    field.mField = &fieldDefinition;
    field.mDefaultValue = "-1";
    Validate().IdField(cls, field);
    cls.mFields.push_back(std::move(field));
  }
}
}
