#include "DotNetModuleFunctionsPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include "generator/lang/BridgingHelper.h"

namespace holgen {
void DotNetModuleFunctionsPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    Process(cls);
    break;
  }
}

void DotNetModuleFunctionsPlugin::Process(Class &cls) {
  for (auto &func: cls.mStruct->mFunctions) {
    auto method = NewFunction(cls, func);
    method.mFunctionPointer = true;
    auto oldArgs = std::move(method.mArguments);
    method.mArguments.clear();
    method.mReturnType =
        BridgingHelper::ConvertType(mProject, method.mReturnType, true, func.mDefinitionSource);
    for (auto &arg: oldArgs) {
      method.mArguments.emplace_back(
          arg.mName,
          BridgingHelper::ConvertType(mProject, arg.mType, false, func.mDefinitionSource),
          arg.mDefaultValue);
    }
    cls.mMethods.push_back(std::move(method));
  }
}
} // namespace holgen
