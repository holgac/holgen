#include "CWrappersPlugin.h"

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
    auto funcArgType = BridgingHelper::ConvertType(mProject, arg.mType, false,
                                                   method.mFunction->mDefinitionSource);
    func.mArguments.emplace_back(arg.mName, funcArgType);
    if (funcArgType.mType == PassByType::Pointer && arg.mType.mType != PassByType::Pointer &&
        funcArgType.mName != "char") {
      args << "*";
    }
    args << arg.mName;
  }

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
