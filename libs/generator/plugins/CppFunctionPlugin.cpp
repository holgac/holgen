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
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
  }
  for (auto &func: structDefinition.mFunctions) {
    if (!func.GetAnnotation(Annotations::LuaFunc))
      AddCppFunction(cls, func);
  }
}

void CppFunctionPlugin::AddCppFunction(Class &cls, const FunctionDefinition &functionDefinition) {
  auto funcAnnotation = functionDefinition.GetAnnotation(Annotations::CppFunc);
  auto method = ClassMethod{
      functionDefinition.mName,
      Type{mProject, functionDefinition.mReturnType},
      Visibility::Public,
      (funcAnnotation && funcAnnotation->GetAttribute(Annotations::CppFunc_Const)) ? Constness::Const
                                                                                   : Constness::NotConst};
  if (funcAnnotation) {
    if (funcAnnotation->GetAttribute(Annotations::CppFunc_OnDestroy)) {
      method.mVisibility = Visibility::Protected;
    } else if (funcAnnotation->GetAttribute(Annotations::CppFunc_Protected)) {
      method.mVisibility = Visibility::Protected;
    } else if (funcAnnotation->GetAttribute(Annotations::CppFunc_Private)) {
      method.mVisibility = Visibility::Private;
    }
  }
  method.mUserDefined = true;
  method.mExposeToLua = functionDefinition.GetAnnotation(Annotations::NoLua) == nullptr;
  method.mFunction = &functionDefinition;

  if (auto cls2 = mProject.GetClass(method.mReturnType.mName)) {
    // TODO: attribute specifying whether const
    if (cls2->mEnum)
      method.mReturnType.mType = PassByType::Value;
    else
      method.mReturnType.mType = PassByType::Pointer;
  }
  // TODO: ref type for complex types
  for (const auto &funcArg: functionDefinition.mArguments) {
    auto &arg = method.mArguments.emplace_back(funcArg.mName, Type{mProject, funcArg.mType});
    if (auto cls2 = mProject.GetClass(arg.mType.mName)) {
      if (cls2->mEnum)
        arg.mType.mType = PassByType::Reference;
      else
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
    arg.mDefaultValue = funcArg.mDefaultValue;
  }
  FillComments(functionDefinition, method.mComments);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
}
