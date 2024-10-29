#include "DotNetPluginBase.h"
#include "generator/utils/CSharpHelper.h"

#include "core/St.h"

namespace holgen {
CSharpMethod DotNetPluginBase::CreateMethod(const Class &cls, const ClassMethod &method,
                                            InteropType interopType, bool addThisArgument,
                                            bool ignoreAuxiliaries) const {
  auto csMethod = CSharpMethod{
      method.mName, CSharpHelper::Get().ConvertType(method.mReturnType, mProject, interopType, true)};
  if (addThisArgument) {
    auto &arg = csMethod.mArguments.emplace_back(
        "holgenObject", CSharpHelper::Get().ConvertType(Type{cls.mName}, mProject, interopType, true));
    if (!cls.IsProxyable())
      arg.mType.mType = CSharpPassByType::Ref;
  }
  PopulateArguments(csMethod.mArguments, method.mArguments, interopType, ignoreAuxiliaries);
  // CSharpHelper::Get().AddAttributes(csMethod.mAttributes, method.mReturnType, csMethod.mReturnType,
  //                                   interopType, true, csMethod.mArguments.size());
  if (!ignoreAuxiliaries) {
    CSharpHelper::Get().AddAuxiliaryArguments(csMethod.mArguments, method.mReturnType,
                                          St::CSharpAuxiliaryReturnValueArgName, interopType, true);
  }
  return csMethod;
}

CSharpConstructor DotNetPluginBase::CreateConstructor(const Class &cls, const ClassMethod &method,
                                                      InteropType interopType) const {
  (void)cls;
  auto csCtor = CSharpConstructor{};
  PopulateArguments(csCtor.mArguments, method.mArguments, interopType, true);
  return csCtor;
}

void DotNetPluginBase::PopulateArguments(std::list<CSharpMethodArgument> &out,
                                         const std::list<MethodArgument> &arguments,
                                         InteropType interopType, bool ignoreAuxiliaries) const {
  for (auto &arg: arguments) {
    auto csType = CSharpHelper::Get().ConvertType(arg.mType, mProject, interopType, false);
    auto &csArg = out.emplace_back(arg.mName, csType, arg.mDefaultValue);
    CSharpHelper::Get().AddAttributes(csArg.mAttributes, arg.mType, csType, interopType, false,
                                      out.size());
    auto argClass = mProject.GetClass(arg.mType.mName);
    if (argClass && !argClass->IsProxyable() && interopType != InteropType::Internal) {
      csArg.mType.mType = CSharpPassByType::Ref;
    }
    if (!ignoreAuxiliaries) {
      CSharpHelper::Get().AddAuxiliaryArguments(out, arg.mType, csArg.mName, interopType, false);
    }
  }
}
} // namespace holgen
