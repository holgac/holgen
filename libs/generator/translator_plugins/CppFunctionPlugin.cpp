#include "CppFunctionPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
  void CppFunctionPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      for (auto &func: cls.mStruct->mFunctions) {
        if (func.GetAnnotation(Annotations::CppFunc))
          AddCppFunction(cls, func);
      }
    }
  }

  void CppFunctionPlugin::AddCppFunction(Class &cls, const FunctionDefinition &func) {
    // TODO: const decorator
    auto &method = cls.mMethods.emplace_back(
        func.mName,
        Type{mProject.mProject, func.mReturnType},
        Visibility::Public, Constness::NotConst);
    method.mUserDefined = true;
    method.mExposeToLua = true;
    for (const auto &arg: func.mArguments) {
      method.mArguments.emplace_back(arg.mName, Type{mProject.mProject, arg.mType});
    }
  }
}
