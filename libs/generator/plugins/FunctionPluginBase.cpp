#include "FunctionPluginBase.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
void FunctionPluginBase::ProcessFunctionArgument(Class &cls, ClassMethod &method,
                                                 const FunctionArgumentDefinition &funcArg) {
  auto &arg = method.mArguments.emplace_back(
      funcArg.mName, Type{mProject, funcArg.mDefinitionSource, funcArg.mType});
  if (funcArg.mType.mName == St::Lua_CustomData) {
    arg.mType = Type{"std::function", PassByType::Reference, Constness::Const};
    arg.mType.mFunctionalTemplateParameters.emplace_back("void");
    arg.mType.mFunctionalTemplateParameters.emplace_back("lua_State", PassByType::Pointer);
    arg.mType.mFunctionalTemplateParameters.emplace_back(cls.mName, PassByType::Reference,
                                                         Constness::Const);
  } else {
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
}

void FunctionPluginBase::ProcessHashFunction(Class &cls, const ClassMethod &method) {
  auto expectedHashFunction = cls.mStruct->GetHashFunction(mProject.mProject);
  THROW_IF(expectedHashFunction != method.mFunction,
           "Class {} ({}) has multiple hash functions: {} and {}", cls.mName,
           cls.mStruct->mDefinitionSource, method.mFunction->mDefinitionSource,
           expectedHashFunction->mDefinitionSource);
  auto funcAnnotation = method.mFunction->GetAnnotation(Annotations::Func);
  THROW_IF(funcAnnotation->GetAttribute(Annotations::LuaFunc),
           "Lua function {} cannot be used as hash", method.mFunction->mDefinitionSource);
  THROW_IF(!method.mArguments.empty(), "Hash function {} cannot take arguments",
           method.mFunction->mDefinitionSource);
  THROW_IF(method.mConstness != Constness::Const, "Hash function {} cannot be non-const",
           method.mFunction->mDefinitionSource);

  auto className = std::format("{}::{}", cls.mNamespace, cls.mName);
  auto hash = Class{"hash", "std"};
  hash.mTemplateSpecializations.push_back(className);
  hash.mClassType = ClassType::Struct;

  auto hasher = ClassMethod{"operator()", Type{"size_t"}, Visibility::Public, Constness::Const};
  hasher.mArguments.emplace_back("obj", Type{className, PassByType::Reference, Constness::Const});
  hasher.mBody.Add("return obj.{}();", method.mName);
  hash.mMethods.push_back(std::move(hasher));
  cls.mSpecializations.push_back(std::move(hash));
}

ClassMethod FunctionPluginBase::NewFunction(Class &cls,
                                            const FunctionDefinition &functionDefinition) {
  auto funcAnnotation = functionDefinition.GetAnnotation(Annotations::Func);
  auto method = ClassMethod{
      functionDefinition.mName,
      Type{mProject, functionDefinition.mDefinitionSource, functionDefinition.mReturnType.mType},
      Visibility::Public,
      (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_Const))
          ? Constness::Const
          : Constness::NotConst};

  if (funcAnnotation) {
    if (funcAnnotation->GetAttribute(Annotations::Func_Public)) {
      method.mVisibility = Visibility::Public;
    } else if (funcAnnotation->GetAttribute(Annotations::Func_OnDestroy)) {
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

  method.mExposeToLua = functionDefinition.GetAnnotation(Annotations::NoLua) == nullptr &&
      method.mVisibility == Visibility::Public;
  method.mFunction = &functionDefinition;

  for (const auto &funcArg: functionDefinition.mArguments) {
    ProcessFunctionArgument(cls, method, funcArg);
  }
  FillComments(functionDefinition, method.mComments);

  if (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_Hash)) {
    ProcessHashFunction(cls, method);
  }

  return method;
}
} // namespace holgen
