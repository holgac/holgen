#include "CppFunctionPlugin.h"
#include "core/Annotations.h"

namespace holgen {
  void CppFunctionPlugin::Run() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct == nullptr)
        continue;
      ProcessStructDefinition(cls, *cls.mStruct);
    }
  }

  void CppFunctionPlugin::ProcessStructDefinition(Class &cls, const StructDefinition &structDefinition) {
    for(auto& mixin: structDefinition.mMixins) {
      ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
    }
    for (auto &func: structDefinition.mFunctions) {
      if (func.GetAnnotation(Annotations::CppFunc))
        AddCppFunction(cls, func);
    }
  }

  void CppFunctionPlugin::AddCppFunction(Class &cls, const FunctionDefinition &func) {
    // TODO: const attribute
    auto method = ClassMethod{
        func.mName,
        Type{mProject, func.mReturnType},
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
      auto &arg = method.mArguments.emplace_back(funcArg.mName, Type{mProject, funcArg.mType});
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
