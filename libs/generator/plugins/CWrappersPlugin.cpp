#include "CWrappersPlugin.h"

#include "core/St.h"

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
  auto func =
      CFunction{Naming().CWrapperName(cls, method), ConvertType(method.mReturnType), &method};

  bool isStatic = method.IsStatic(cls);

  if (!isStatic) {
    func.mArguments.emplace_back("instance", ConvertType(Type{cls.mName, PassByType::Pointer}));
  }

  std::stringstream args;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst)
      isFirst = false;
    else
      args << ", ";
    func.mArguments.emplace_back(arg.mName, ConvertType(arg.mType));
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

  func.mBody.Add("return {}{}({});", prefix, method.mName, args.str());

  cls.mCFunctions.push_back(std::move(func));
}

Type CWrappersPlugin::ConvertType(const Type &type) {
  return type;
}
} // namespace holgen
