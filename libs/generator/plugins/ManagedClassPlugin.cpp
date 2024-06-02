#include "ManagedClassPlugin.h"
#include "../NamingConvention.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void ManagedClassPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr)
      continue;
    auto managedAnnotation = cls.mStruct->GetAnnotation(Annotations::Managed);
    if (managedAnnotation == nullptr)
      continue;
    Validate().ManagedAnnotation(cls, *managedAnnotation);

    auto manager =
        mProject.GetClass(managedAnnotation->GetAttribute(Annotations::Managed_By)->mValue.mName);
    auto managerField = manager->GetFieldFromDefinitionName(
        managedAnnotation->GetAttribute(Annotations::Managed_Field)->mValue.mName);

    cls.mSourceIncludes.AddLocalHeader(St::GlobalPointer + ".h");
    cls.mSourceIncludes.AddLocalHeader(manager->mName + ".h");
    GenerateStaticGetter(cls, *manager, *managerField);
    for (const auto &annotation: managerField->mField->GetAnnotations(Annotations::Index)) {
      GenerateIndexGetter(cls, *manager, *managerField, annotation);
    }
  }
}

void ManagedClassPlugin::GenerateStaticGetter(Class &cls, const Class &manager,
                                              const ClassField &managerField) const {
  auto method = ClassMethod{St::ManagedObject_Getter,
                            Type{cls.mName, PassByType::Pointer, Constness::NotConst},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("id", Type{mProject, cls.GetIdField()->mField->mType});
  method.mBody.Add("return {}<{}>::GetInstance()->{}(id);", St::GlobalPointer, manager.mName,
                   Naming().ContainerElemGetterNameInCpp(*managerField.mField));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void ManagedClassPlugin::GenerateIndexGetter(Class &cls, const Class &manager,
                                             const ClassField &managerField,
                                             const AnnotationDefinition &annotation) const {
  auto method = ClassMethod{Naming().ManagedClassIndexGetterNameInCpp(annotation),
                            Type{cls.mStruct->mName, PassByType::Pointer, Constness::NotConst},
                            Visibility::Public, Constness::NotConst, Staticness::Static};
  auto indexedOnField =
      cls.GetFieldFromDefinitionName(annotation.GetAttribute(Annotations::Index_On)->mValue.mName);
  ClassMethodArgument &keyArg =
      method.mArguments.emplace_back("key", Type{mProject, indexedOnField->mField->mType});
  keyArg.mType.PreventCopying();
  method.mBody.Add("return {}<{}>::GetInstance()->{}(key);", St::GlobalPointer, manager.mName,
                   Naming().ContainerIndexGetterNameInCpp(*managerField.mField, annotation));
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
