#include "ManagedClassPlugin.h"
#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
  void ManagedClassPlugin::EnrichClasses() {
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
        auto &getter = generatedClass.mMethods.emplace_back();
        getter.mName = St::ManagedObject_Getter;
        getter.mStaticness = Staticness::Static;
        getter.mConstness = Constness::NotConst;
        getter.mReturnType.mName = generatedClass.mStruct->mName;
        getter.mReturnType.mType = PassByType::Pointer;
        getter.mReturnType.mConstness = Constness::NotConst;
        auto &idArg = getter.mArguments.emplace_back();
        TypeInfo::Get().ConvertToType(idArg.mType, idField->mType);
        idArg.mName = "id";
        getter.mBody.Add("return {}<{}>::GetInstance()->{}(id);",
                         St::GlobalPointer, manager->mName,
                         St::GetGetterMethodName(managerFieldContainerElemNameAttribute->mValue.mName));
        generatedClass.mSourceIncludes.AddLocalHeader(St::GlobalPointer + ".h");
        generatedClass.mSourceIncludes.AddLocalHeader(manager->mName + ".h");
      }

      for (auto &annotation: managerField->mAnnotations) {
        if (annotation.mName != Annotations::Index)
          continue;

        auto &getter = generatedClass.mMethods.emplace_back();
        auto indexOn = annotation.GetAttribute(Annotations::Index_On);
        getter.mName = St::GetIndexGetterName("", indexOn->mValue.mName);
        getter.mStaticness = Staticness::Static;
        getter.mConstness = Constness::NotConst;
        getter.mReturnType.mName = generatedClass.mStruct->mName;
        getter.mReturnType.mType = PassByType::Pointer;
        getter.mReturnType.mConstness = Constness::NotConst;
        ClassMethodArgument &valArg = getter.mArguments.emplace_back();
        auto indexedOnField = generatedClass.mStruct->GetField(indexOn->mValue.mName);
        TypeInfo::Get().ConvertToType(valArg.mType, indexedOnField->mType);
        if (!TypeInfo::Get().CppPrimitives.contains(valArg.mType.mName)) {
          valArg.mType.mType = PassByType::Reference;
          valArg.mType.mConstness = Constness::Const;
        }
        valArg.mName = "val";
        getter.mBody.Add(
            "return {}<{}>::GetInstance()->{}(val);",
            St::GlobalPointer, manager->mName,
            St::GetIndexGetterName(managerFieldContainerElemNameAttribute->mValue.mName, indexOn->mValue.mName));

      }

    }
  }
}
