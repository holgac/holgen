#include "ManagedClassPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

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
      auto managerFieldContainerAnnotation = managerField->GetAnnotation(Annotations::Container);
      auto managerFieldContainerElemNameAttribute = managerFieldContainerAnnotation->GetAttribute(
          Annotations::Container_ElemName);
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
                         St::GetGetterMethodName(managerFieldContainerElemNameAttribute->mValue.mName));
        generatedClass.mSourceIncludes.AddLocalHeader(St::GlobalPointer + ".h");
        generatedClass.mSourceIncludes.AddLocalHeader(manager->mName + ".h");
      }

      for (auto &annotation: managerField->mAnnotations) {
        if (annotation.mName != Annotations::Index)
          continue;

        auto indexOn = annotation.GetAttribute(Annotations::Index_On);
        auto &getter = generatedClass.mMethods.emplace_back(
            St::GetIndexGetterName("", indexOn->mValue.mName),
            Type{generatedClass.mStruct->mName, PassByType::Pointer, Constness::NotConst},
            Visibility::Public,
            Constness::NotConst,
            Staticness::Static
            );
        auto indexedOnField = generatedClass.mStruct->GetField(indexOn->mValue.mName);
        ClassMethodArgument &valArg = getter.mArguments.emplace_back("val", Type{mProject.mProject, indexedOnField->mType});
        if (!TypeInfo::Get().CppPrimitives.contains(valArg.mType.mName)) {
          valArg.mType.mType = PassByType::Reference;
          valArg.mType.mConstness = Constness::Const;
        }
        getter.mBody.Add(
            "return {}<{}>::GetInstance()->{}(val);",
            St::GlobalPointer, manager->mName,
            St::GetIndexGetterName(managerFieldContainerElemNameAttribute->mValue.mName, indexOn->mValue.mName));

      }

    }
  }
}