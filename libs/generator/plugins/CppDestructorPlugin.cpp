#include "CppDestructorPlugin.h"
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

  void CppDestructorPlugin::ProcessStructDefinition(Class& cls, ClassDestructor& destructor, const StructDefinition& structDefinition) {
    for(auto& mixin: structDefinition.mMixins) {
      ProcessStructDefinition(cls, destructor, *mProject.mProject.GetStruct(mixin));
    }
    for(auto& func: structDefinition.mFunctions) {
      if (!func.GetAnnotation(Annotations::CppFunc))
        continue;
      if (func.GetAnnotation(Annotations::CppFunc)->GetAttribute(Annotations::CppFunc_OnDestroy)) {
        destructor.mBody.Add("{}();", func.mName);
      }
    }
  }
}
