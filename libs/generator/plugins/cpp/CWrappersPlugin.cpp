#include "CWrappersPlugin.h"
#include <iostream>

#include "core/St.h"
#include "generator/lang/BridgingHelper.h"

namespace holgen {
void CWrappersPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct)
      continue;
    if (cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    ProcessClass(cls);
  }
}

void CWrappersPlugin::ProcessClass(Class &cls) {
  for (auto &method: cls.mMethods) {
    if (!method.mExposeToScript)
      continue;
    WrapMethod(cls, method);
  }
}

void CWrappersPlugin::WrapMethod(Class &cls, const ClassMethod &method) {
  auto func = CFunction{Naming().CWrapperName(cls, method),
                        BridgingHelper::ConvertType(mProject, method.mReturnType, true,
                                                    method.mFunction->mDefinitionSource),
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
    auto &addedArg =
        BridgingHelper::AddArgument(mProject, func, arg, method.mFunction->mDefinitionSource);
    bool isSpan = arg.mType.mName == "std::span";
    bool isPointer = arg.mType.mType == PassByType::Pointer;

    if (isSpan && !isPointer && arg.mType.mTemplateParameters.front().mName != "std::string") {
      args << std::format("std::span{{{0}, {0}{1}}}", arg.mName, St::CSharpAuxiliarySizeSuffix);
      continue;
    }

    bool isVector = arg.mType.mName == "std::vector";
    if (isSpan || isVector) {
      func.mBody.Add("std::vector<{}> {}HolgenVector;",
                     arg.mType.mTemplateParameters.front().ToString(true, false), arg.mName);
      func.mBody.Add("{0}HolgenVector.reserve({0}{1});", arg.mName, St::CSharpAuxiliarySizeSuffix);
      func.mBody.Add("for (size_t i = 0; i < {}{}; ++i) {{", arg.mName,
                     St::CSharpAuxiliarySizeSuffix);
      func.mBody.Indent(1);
      func.mBody.Add("{0}HolgenVector.emplace_back({0}[i]);", arg.mName);
      func.mBody.Indent(-1);
      func.mBody.Add("}}");
      if (isSpan)
        args << std::format("std::span{{{0}HolgenVector.data(), {0}HolgenVector.size()}}", arg.mName);
      else
        args << std::format("{0}HolgenVector", arg.mName);
      continue;
    }
    if (addedArg.mType.mType == PassByType::Pointer && arg.mType.mType != PassByType::Pointer &&
        addedArg.mType.mName != "char") {
      args << "*";
    }
    args << arg.mName;
  }

  BridgingHelper::AddAuxiliaryArguments(mProject, func, method.mReturnType,
                                        St::CSharpAuxiliaryReturnTypeArgName);

  std::string prefix;
  bool isSingleton = cls.mStruct && cls.mStruct->GetAnnotation(Annotations::Singleton);
  if (isSingleton) {
    prefix = std::format("{}::{}::GetInstance().", cls.mNamespace, cls.mName);
  } else if (isStatic) {
    prefix = std::format("{}::{}::", cls.mNamespace, cls.mName);
  } else {
    prefix = "instance->";
  }
  bool pointerMismatch = func.mReturnType.mType == PassByType::Pointer &&
      method.mReturnType.mType != PassByType::Pointer;

  func.mBody.Add("return {}{}{}({});", pointerMismatch ? "&" : "", prefix, method.mName,
                 args.str());

  cls.mCFunctions.push_back(std::move(func));
}
} // namespace holgen
