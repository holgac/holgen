#include "ManagedClassPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"
#include "../NamingConvention.h"

namespace holgen {
  void ManagedClassPlugin::Run() {
    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mStruct == nullptr)
        continue;
      auto managedAnnotation = generatedClass.mStruct->GetAnnotation(Annotations::Managed);
      if (managedAnnotation == nullptr)
        continue;

      auto managedByAttribute = managedAnnotation->GetAttribute(Annotations::Managed_By);
      auto managedFieldAttribute = managedAnnotation->GetAttribute(Annotations::Managed_Field);
      auto manager = mProject.mProject.GetStruct(managedByAttribute->mValue.mName);
      auto managerField = manager->GetField(managedFieldAttribute->mValue.mName);
      auto idField = generatedClass.mStruct->GetIdField();

      {
        auto &getter = generatedClass.mMethods.emplace_back(
            St::ManagedObject_Getter,
            Type{generatedClass.mStruct->mName, PassByType::Pointer, Constness::NotConst},
            Visibility::Public,
            Constness::NotConst,
            Staticness::Static
        );
        getter.mArguments.emplace_back("id", Type{mProject.mProject, idField->mType});
        getter.mBody.Add("return {}<{}>::GetInstance()->{}(id);",
                         St::GlobalPointer, manager->mName,
                         Naming().ContainerElemGetterNameInCpp(*managerField));
        generatedClass.mSourceIncludes.AddLocalHeader(St::GlobalPointer + ".h");
        generatedClass.mSourceIncludes.AddLocalHeader(manager->mName + ".h");
      }

      for (const auto &annotation: managerField->GetAnnotations(Annotations::Index)) {
        auto indexOn = annotation.GetAttribute(Annotations::Index_On);
        auto &getter = generatedClass.mMethods.emplace_back(
            Naming().ManagedClassIndexGetterNameInCpp(annotation),
            Type{generatedClass.mStruct->mName, PassByType::Pointer, Constness::NotConst},
            Visibility::Public,
            Constness::NotConst,
            Staticness::Static
        );
        auto indexedOnField = generatedClass.mStruct->GetField(indexOn->mValue.mName);
        ClassMethodArgument &keyArg = getter.mArguments.emplace_back("key",
                                                                     Type{mProject.mProject, indexedOnField->mType});
        keyArg.mType.PreventCopying();
        getter.mBody.Add(
            "return {}<{}>::GetInstance()->{}(key);",
            St::GlobalPointer, manager->mName,
            Naming().ContainerIndexGetterNameInCpp(*managerField, annotation));

      }

    }
  }
}
