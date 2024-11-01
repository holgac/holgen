#include "CWrappersPlugin.h"
#include <iostream>

#include "core/St.h"
#include "generator/utils/BridgingHelper.h"

namespace holgen {
void CWrappersPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::CSharp))
    return;
  for (auto &cls: mProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    ProcessClass(cls);
  }
}

bool CWrappersPlugin::ShouldProcess(const Class &cls) const {
  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::DotNetModule))
    return false;
  return true;
}

void CWrappersPlugin::ProcessClass(Class &cls) const {
  for (auto &method: cls.mMethods) {
    if (!method.mExposeToCSharp)
      continue;
    WrapMethod(cls, method);
  }
}

void CWrappersPlugin::WrapMethod(Class &cls, const ClassMethod &method) const {
  DefinitionSource internalSource{"INTERNAL"};
  auto func =
      CFunction{Naming().CWrapperName(cls, method),
                BridgingHelper::ConvertType(mProject, method.mReturnType, true,
                                            method.mFunction ? method.mFunction->mDefinitionSource
                                                             : internalSource),
                &method};

  bool isStatic = method.IsStatic(cls);

  if (!isStatic) {
    func.mArguments.emplace_back("instance",
                                 Type{cls.mName, PassByType::Pointer, method.mConstness});
  }

  std::stringstream args;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst)
      isFirst = false;
    else
      args << ", ";
    auto &addedArg = BridgingHelper::AddArgument(
        mProject, func, arg,
        method.mFunction ? method.mFunction->mDefinitionSource : internalSource);
    bool isSpan = arg.mType.mName == "std::span";
    bool isPointer = arg.mType.mType == PassByType::Pointer;

    if (isSpan && !isPointer && arg.mType.mTemplateParameters.front().mName != "std::string") {
      args << std::format("std::span{{{0}, {0}{1}}}", arg.mName, St::CSharpAuxiliarySizeSuffix);
      continue;
    }

    bool isVector = arg.mType.mName == "std::vector";
    if (isSpan || isVector) {
      func.mBody.Add("std::vector<{}> {}HolgenVector;",
                     arg.mType.mTemplateParameters.front().ToFullyQualifiedString(mProject),
                     arg.mName);
      func.mBody.Add("{0}HolgenVector.reserve({0}{1});", arg.mName, St::CSharpAuxiliarySizeSuffix);
      func.mBody.Add("for (size_t i = 0; i < {}{}; ++i) {{", arg.mName,
                     St::CSharpAuxiliarySizeSuffix);
      func.mBody.Indent(1);
      func.mBody.Add("{0}HolgenVector.emplace_back({0}[i]);", arg.mName);
      func.mBody.Indent(-1);
      func.mBody.Add("}}");
      if (isSpan)
        args << std::format("std::span{{{0}HolgenVector.data(), {0}HolgenVector.size()}}",
                            arg.mName);
      else
        args << std::format("{0}HolgenVector", arg.mName);
      continue;
    }
    auto argClass = mProject.GetClass(addedArg.mType.mName);
    if (argClass && argClass->mStruct && argClass->mStruct->GetAnnotation(Annotations::Interface)) {
      addedArg.mType = Type{"void", PassByType::Pointer};
      args << std::format("{}::{}({})", argClass->mNamespace, argClass->mName, arg.mName);
    } else {
      if (addedArg.mType.mType == PassByType::Pointer && arg.mType.mType != PassByType::Pointer &&
          addedArg.mType.mName != "char") {
        args << "*";
      }
      args << arg.mName;
    }
  }

  BridgingHelper::AddAuxiliaryArguments(mProject, func, method.mReturnType,
                                        St::CSharpAuxiliaryReturnValueArgName, true);

  std::string prefix;
  bool isSingleton = cls.mStruct && cls.mStruct->GetAnnotation(Annotations::Singleton);
  if (isSingleton) {
    prefix = std::format("{}::{}::GetInstance().", cls.mNamespace, cls.mName);
  } else if (isStatic) {
    prefix = std::format("{}::{}::", cls.mNamespace, cls.mName);
  } else {
    prefix = "instance->";
  }
  bool isContainer = func.mReturnType.mName == "std::span" ||
      func.mReturnType.mName == "std::array" || func.mReturnType.mName == "std::vector";
  bool pointerMismatch = func.mReturnType.mType == PassByType::Pointer &&
      method.mReturnType.mType != PassByType::Pointer && !isContainer;
  auto funcCall = std::format("{}{}({})", prefix, method.mName, args.str());
  if (method.mReturnType.mName == "void") {
    func.mBody.Add("{};", funcCall);
  } else if (method.mReturnType.mName == "std::array" ||
             method.mReturnType.mName == "std::vector" || method.mReturnType.mName == "std::span") {
    cls.mHeaderIncludes.AddLocalHeader("DeferredDeleter.h");
    cls.mHeaderIncludes.AddStandardHeader("algorithm");
    func.mBody.Add("using DeferredDeleter = {}::DeferredDeleter;", mSettings.mNamespace);
    func.mBody.Add("constexpr size_t BufferSize = sizeof(DeferredDeleter) + "
                   "(sizeof(DeferredDeleter)%8) + sizeof({});",
                   SanitizeType(method.mReturnType).ToFullyQualifiedString(mProject));
    func.mBody.Add(
        "constexpr size_t ObjectOffset = sizeof(DeferredDeleter) + (sizeof(DeferredDeleter)%8);");
    func.mBody.Add("auto buffer = new char[BufferSize];");
    bool canMove = method.mReturnType.mName == "std::vector" &&
        method.mReturnType.mConstness != Constness::Const;
    func.mBody.Add("auto holgenRes = new (&buffer[ObjectOffset]) {}({}{}{});",
                   SanitizeType(method.mReturnType).ToFullyQualifiedString(mProject),
                   canMove ? "std::move(" : "", funcCall, canMove ? ")" : "");
    func.mBody.Add("auto deferredDeleter = new (buffer) DeferredDeleter([](void* ptr) {{ ");
    func.mBody.Indent(1);
    func.mBody.Add("static_cast<{0}*>(ptr)->~{1}();",
                   SanitizeType(method.mReturnType).ToFullyQualifiedString(mProject),
                   St::StripNamespace(method.mReturnType.mName));
    func.mBody.Indent(-1);
    func.mBody.Add("}});");
    if (method.mReturnType.mName == "std::vector" || method.mReturnType.mName == "std::span") {
      func.mBody.Add("*{}{} = holgenRes->size();", St::CSharpAuxiliaryReturnValueArgName,
                     St::CSharpAuxiliarySizeSuffix);
    }

    auto &arg = func.mArguments.emplace_back(St::DeferredDeleterArgumentName,
                                             Type{"DeferredDeleter", PassByType::Pointer});
    ++arg.mType.mPointerDepth;

    func.mBody.Add("*{} = deferredDeleter;", St::DeferredDeleterArgumentName);
    func.mBody.Add("return holgenRes->data();");
    func.mHasDeferredDeleter = true;
  } else {
    func.mBody.Add("return {}{};", pointerMismatch ? "&" : "", funcCall);
  }

  cls.mCFunctions.push_back(std::move(func));
}

Type CWrappersPlugin::SanitizeType(const Type &type) const {
  Type res = type;
  if (res.mType == PassByType::Reference)
    res.mType = PassByType::Value;
  res.mConstness = Constness::NotConst;
  return res;
}
} // namespace holgen
