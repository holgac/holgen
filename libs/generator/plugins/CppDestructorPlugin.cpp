#include "CppDestructorPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
  void CppDestructorPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      ClassDestructor destructor;
      ProcessStructDefinition(cls, destructor, *cls.mStruct);
      if (!destructor.mBody.mContents.empty()) {
        cls.mDestructor = std::move(destructor);
      }
    }
  }

  void CppDestructorPlugin::ProcessStructDefinition(Class &cls, ClassDestructor &destructor,
                                                    const StructDefinition &structDefinition) {
    for (auto &mixin: structDefinition.mMixins) {
      ProcessStructDefinition(cls, destructor, *mProject.mProject.GetStruct(mixin));
    }
    for (auto &func: structDefinition.mFunctions) {
      if (!func.GetAnnotation(Annotations::CppFunc))
        continue;
      if (func.GetAnnotation(Annotations::CppFunc)->GetAttribute(Annotations::CppFunc_OnDestroy)) {
        destructor.mBody.Add("{}();", func.mName);
      }
    }
    for (auto &field: structDefinition.mFields) {
      if (field.mType.mName == St::Variant)
        ProcessVariantField(cls, destructor, field);
    }
  }

  void CppDestructorPlugin::ProcessVariantField(
      Class &cls, ClassDestructor &destructor, const FieldDefinition &fieldDefinition
  ) {
    auto typeField = cls.GetField(Naming().FieldNameInCpp(
        fieldDefinition.GetAnnotation(Annotations::Variant)->GetAttribute(
            Annotations::Variant_TypeField)->mValue.mName));
    bool isFirst = true;
    for (auto &projectStruct: mProject.mProject.mStructs) {
      if (projectStruct.mIsMixin)
        continue;
      auto structVariantAnnotation = projectStruct.GetAnnotation(Annotations::Variant);
      if (!structVariantAnnotation ||
          structVariantAnnotation->GetAttribute(Annotations::Variant_Enum)->mValue.mName != typeField->mType.mName)
        continue;
      auto entryAttribute = structVariantAnnotation->GetAttribute(Annotations::Variant_Entry);
      if (isFirst) {
        isFirst = false;
        destructor.mBody.Add(
            "if ({} == {}::{}) {{", typeField->mName, typeField->mType.mName, entryAttribute->mValue.mName);
      } else {
        destructor.mBody.Add(
            "}} else if ({} == {}::{}) {{", typeField->mName, typeField->mType.mName, entryAttribute->mValue.mName);
      }
      destructor.mBody.Indent(1);
      destructor.mBody.Add("{}()->~{}();", Naming().VariantGetterNameInCpp(fieldDefinition, projectStruct), projectStruct.mName);
      destructor.mBody.Indent(-1);
    }
    destructor.mBody.Add("}}");
  }
}
