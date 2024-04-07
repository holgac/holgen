#include "CppFunctionPlugin.h"
#include "core/Annotations.h"

namespace holgen {
  void CppFunctionPlugin::Run() {
    // TODO: mixins
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
    // TODO: const attribute
    auto method = ClassMethod{
        func.mName,
        Type{mProject.mProject, func.mReturnType},
        Visibility::Public, Constness::NotConst};
    method.mUserDefined = true;
    method.mExposeToLua = true;
    method.mFunction = &func;

    if (mProject.GetClass(method.mReturnType.mName)) {
      // TODO: attribute specifying whether const
      method.mReturnType.mType = PassByType::Pointer;
    }
    // TODO: ref type for complex types
    for (const auto &funcArg: func.mArguments) {
      auto &arg = method.mArguments.emplace_back(funcArg.mName, Type{mProject.mProject, funcArg.mType});
      if (mProject.GetClass(arg.mType.mName)) {
        arg.mType.mType = PassByType::Pointer;
        if (!funcArg.mIsOut)
          arg.mType.mConstness = Constness::Const;
      } else if (!TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
        arg.mType.mType = PassByType::Reference;
        if (!funcArg.mIsOut)
          arg.mType.mConstness = Constness::Const;
      } else if (funcArg.mIsOut) {
        arg.mType.mType = PassByType::Reference;
      }
    }
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}
