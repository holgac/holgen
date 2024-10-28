#include "CppDestructorPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void CppDestructorPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    ProcessStructDefinition(cls, cls.mDestructor, *cls.mStruct);
  }
}

void CppDestructorPlugin::ProcessStructDefinition(Class &cls, ClassDestructor &destructor,
                                                  const StructDefinition &structDefinition) {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, destructor, *mProject.mProject.GetStruct(mixin));
  }
  for (auto &func: structDefinition.mFunctions) {
    if (!func.GetAnnotation(Annotations::Func))
      continue;
    if (func.GetAnnotation(Annotations::Func)->GetAttribute(Annotations::Func_OnDestroy)) {
      destructor.mBody.Add("{}();", func.mName);
    }
  }
  ProcessVariantFields(destructor, structDefinition);
}

void CppDestructorPlugin::ProcessVariantFields(ClassDestructor &destructor,
                                               const StructDefinition &structDefinition) const {
  std::set<std::string> processedTypeFields;
  for (auto &field: structDefinition.mFields) {
    if (field.mType.mName != St::Variant)
      continue;
    auto typeFieldName = field.GetAnnotation(Annotations::Variant)
                             ->GetAttribute(Annotations::Variant_TypeField)
                             ->mValue.mName;
    if (processedTypeFields.contains(typeFieldName))
      continue;
    processedTypeFields.insert(typeFieldName);
    destructor.mBody.Add("{}();", Naming().VariantResetterNameInCpp(typeFieldName));
  }
}
} // namespace holgen
