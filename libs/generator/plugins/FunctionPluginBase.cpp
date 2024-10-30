#include "FunctionPluginBase.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {
void FunctionPluginBase::ProcessFunctionArgument(MethodBase &method,
                                                 const FunctionArgumentDefinition &funcArg) {
  auto &arg = method.mArguments.emplace_back(
      funcArg.mName, Type{mProject, funcArg.mDefinitionSource, funcArg.mType});
  if (funcArg.mType.mName == St::Lua_CustomData) {
    arg.mType = Type{"std::function", PassByType::Reference, Constness::Const};
    arg.mType.mFunctionalTemplateParameters.emplace_back("void");
    arg.mType.mFunctionalTemplateParameters.emplace_back("lua_State", PassByType::Pointer);
  } else {
    arg.mType.mConstness = funcArg.mConstness;
    if (TypeInfo::Get().CppPrimitives.contains(arg.mType.mName)) {
      auto pbt = funcArg.mConstness == Constness::Const ? PassByType::Value : PassByType::Reference;
      arg.mType.mType = funcArg.mNullability == Nullability::Nullable ? PassByType::Pointer : pbt;
    } else if (funcArg.mNewness != Newness::New) {
      arg.mType.mType = funcArg.mNullability == Nullability::Nullable ? PassByType::Pointer
                                                                      : PassByType::Reference;
    }
    arg.mDefaultValue = funcArg.mDefaultValue;
  }
}

void FunctionPluginBase::ProcessHashFunction(Class &cls, ClassMethod &method) {
  method.mConstness = Constness::Const;
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

void FunctionPluginBase::ProcessToStringFunction(Class &cls, ClassMethod &method) {
  method.mConstness = Constness::Const;
  auto expectedToStringFunction = cls.mStruct->GetToStringFunction(mProject.mProject);
  THROW_IF(expectedToStringFunction != method.mFunction,
           "Class {} ({}) has multiple toString functions: {} and {}", cls.mName,
           cls.mStruct->mDefinitionSource, method.mFunction->mDefinitionSource,
           expectedToStringFunction->mDefinitionSource);
  cls.mHeaderIncludes.AddStandardHeader("format");
  auto className = std::format("{}::{}", cls.mNamespace, cls.mName);
  auto formatter = Class{"formatter", "std"};
  formatter.mTemplateSpecializations.push_back(className);
  formatter.mClassType = ClassType::Struct;

  auto &base = formatter.mBaseClasses.emplace_back(Visibility::Public, Type{"formatter"});
  base.mType.mTemplateParameters.emplace_back("string");

  auto format = ClassMethod{"format", Type{"auto"}, Visibility::Public, Constness::Const};
  format.mTemplateParameters.emplace_back("typename", "FormatContext");
  format.mArguments.emplace_back("obj", Type{className, PassByType::Reference, Constness::Const});
  format.mArguments.emplace_back("ctx",
                                 Type{"FormatContext", PassByType::Reference, Constness::NotConst});
  format.mBody.Add("return format_to(ctx.out(), \"{{}}\", obj.{}());", method.mName);
  formatter.mMethods.push_back(std::move(format));
  cls.mSpecializations.push_back(std::move(formatter));
}

void FunctionPluginBase::ProcessMethodVisibility(MethodBase &method,
                                                 const FunctionDefinition &functionDefinition) {
  auto funcAnnotation = functionDefinition.GetAnnotation(Annotations::Func);
  if (!funcAnnotation) {
    return;
  }
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

ClassMethod FunctionPluginBase::NewFunction(Class &cls,
                                            const FunctionDefinition &functionDefinition) {
  auto funcAnnotation = functionDefinition.GetAnnotation(Annotations::Func);
  auto method = ClassMethod{
      functionDefinition.mName, Type::ReturnType(mProject, functionDefinition), Visibility::Public,
      (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_Const))
          ? Constness::Const
          : Constness::NotConst};
  ProcessMethodVisibility(method, functionDefinition);

  method.mReturnType.mConstness = functionDefinition.mReturnType.mConstness;

  method.mExposeToScript = functionDefinition.GetAnnotation(Annotations::NoLua) == nullptr &&
      method.mVisibility == Visibility::Public;
  method.mFunction = &functionDefinition;

  for (const auto &funcArg: functionDefinition.mArguments) {
    ProcessFunctionArgument(method, funcArg);
  }
  FillComments(functionDefinition, method.mComments);

  if (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_Hash)) {
    ProcessHashFunction(cls, method);
  }

  if (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_ToString)) {
    ProcessToStringFunction(cls, method);
  }

  if (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_Virtual)) {
    method.mVirtuality = Virtuality::Virtual;
  } else if (funcAnnotation && funcAnnotation->GetAttribute(Annotations::Func_PureVirtual)) {
    method.mVirtuality = Virtuality::PureVirtual;
  }

  return method;
}

ClassConstructor FunctionPluginBase::NewConstructor(Class &cls,
                                                    const FunctionDefinition &functionDefinition) {
  (void)cls;
  auto ctor = ClassConstructor{};
  ProcessMethodVisibility(ctor, functionDefinition);

  for (const auto &funcArg: functionDefinition.mArguments) {
    ProcessFunctionArgument(ctor, funcArg);
  }
  FillComments(functionDefinition, ctor.mComments);
  ctor.mFunction = &functionDefinition;

  return ctor;
}
} // namespace holgen
