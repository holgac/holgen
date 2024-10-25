#include "CWrappersPlugin.h"

#include "core/St.h"

namespace holgen {
void CWrappersPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct)
      continue;
    auto singleton = cls.mStruct->GetAnnotation(Annotations::Singleton);
    ProcessClass(cls, singleton);
  }
}

void CWrappersPlugin::ProcessClass(Class &cls, bool singleton) {
  for (auto &method: cls.mMethods) {
    if (!method.mExposeToScript)
      continue;
    WrapMethod(cls, method, singleton);
  }
}

void CWrappersPlugin::WrapMethod(Class &cls, const ClassMethod &method, bool singleton) {
  auto func = CFunction{
      std::format("{}_{}_{}", St::Replace(cls.mNamespace, "::", "_"), cls.mName, method.mName),
      ConvertType(method.mReturnType), &method};

  if (!singleton) {
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
  if (singleton) {
    prefix = std::format("{}::{}::GetInstance().", cls.mNamespace, cls.mName);
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
