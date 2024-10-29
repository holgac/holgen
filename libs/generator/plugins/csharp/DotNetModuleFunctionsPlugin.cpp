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
    for (auto &arg: oldArgs) {
      BridgingHelper::AddArgument(mProject, method, arg, func.mDefinitionSource);
    }
    BridgingHelper::AddAuxiliaryArguments(mProject, method, method.mReturnType,
                                          St::CSharpAuxiliaryReturnValueArgName, true);
    method.mReturnType =
        BridgingHelper::ConvertType(mProject, method.mReturnType, true, func.mDefinitionSource);
    cls.mMethods.push_back(std::move(method));
  }
}
} // namespace holgen
