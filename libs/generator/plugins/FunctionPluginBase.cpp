#include "FunctionPluginBase.h"

namespace holgen {
ClassMethod FunctionPluginBase::NewFunction(const FunctionDefinition &functionDefinition) {
  auto funcAnnotation = functionDefinition.GetAnnotation(Annotations::Func);
  auto method = ClassMethod{
      functionDefinition.mName,
      Type{mProject, functionDefinition.mDefinitionSource, functionDefinition.mReturnType.mType},
      Visibility::Public,
      (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_Const))
          ? Constness::Const
          : Constness::NotConst};
  if (funcAnnotation) {
    if (funcAnnotation->GetAttribute(Annotations::Func_OnDestroy)) {
      method.mVisibility = Visibility::Protected;
    } else if (funcAnnotation->GetAttribute(Annotations::Func_Protected)) {
      method.mVisibility = Visibility::Protected;
    } else if (funcAnnotation->GetAttribute(Annotations::Func_Private)) {
      method.mVisibility = Visibility::Private;
    }
  }

  switch (functionDefinition.mReturnType.mCategory) {
  case FunctionReturnTypeCategory::Pointer:
    method.mReturnType.mType = PassByType::Pointer;
    break;
  case FunctionReturnTypeCategory::Reference:
    method.mReturnType.mType = PassByType::Reference;
    break;
  case FunctionReturnTypeCategory::NewObject:
    method.mReturnType.mType = PassByType::Value;
    break;
  }

  method.mReturnType.mConstness = functionDefinition.mReturnType.mConstness;

  method.mExposeToLua = functionDefinition.GetAnnotation(Annotations::NoLua) == nullptr;
  method.mFunction = &functionDefinition;

  for (const auto &funcArg: functionDefinition.mArguments) {
    auto &arg = method.mArguments.emplace_back(
        funcArg.mName, Type{mProject, funcArg.mDefinitionSource, funcArg.mType});
    arg.mType.mConstness = funcArg.mConstness;
    if (TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
      auto pbt = funcArg.mConstness == Constness::Const ? PassByType::Value : PassByType::Reference;
      arg.mType.mType = funcArg.mNullability == Nullability::Nullable ? PassByType::Pointer : pbt;
    } else {
      arg.mType.mType = funcArg.mNullability == Nullability::Nullable ? PassByType::Pointer
                                                                      : PassByType::Reference;
    }
    arg.mDefaultValue = funcArg.mDefaultValue;
  }
  FillComments(functionDefinition, method.mComments);
  return method;
}
} // namespace holgen
