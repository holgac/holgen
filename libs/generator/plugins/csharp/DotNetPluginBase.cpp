#include "DotNetPluginBase.h"
#include "generator/lang/CSharpHelper.h"

namespace holgen {
CSharpMethod DotNetPluginBase::CreateMethod(const ClassMethod &method,
                                            InteropType interopType) const {
  auto csMethod = CSharpMethod{
      method.mName, CSharpHelper::Get().ConvertType(method.mReturnType, mProject, interopType)};
  std::size_t argIdx = 0;
  for (auto &arg: method.mArguments) {
    auto csType = CSharpHelper::Get().ConvertType(arg.mType, mProject, interopType);
    auto &csArg = csMethod.mArguments.emplace_back(arg.mName, csType, arg.mDefaultValue);
    CSharpHelper::Get().AddAttributes(csArg.mAttributes, arg.mType, csType, interopType, false,
                                      argIdx + 1);
    ++argIdx;
  }
  CSharpHelper::Get().AddAttributes(csMethod.mAttributes, method.mReturnType, csMethod.mReturnType,
                                    interopType, false, csMethod.mArguments.size() - 1);
  return csMethod;
}
} // namespace holgen
