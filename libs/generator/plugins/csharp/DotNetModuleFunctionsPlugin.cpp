#include "DotNetModuleFunctionsPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

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
    cls.mMethods.push_back(std::move(method));
  }
}
} // namespace holgen
