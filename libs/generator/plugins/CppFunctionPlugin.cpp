#include "CppFunctionPlugin.h"
#include "core/Annotations.h"

namespace holgen {
void CppFunctionPlugin::Run() {
  for (auto &cls: mProject.mClasses) {
    if (cls.mStruct == nullptr || cls.mStruct->GetAnnotation(Annotations::LuaFuncTable))
      continue;
    ProcessStructDefinition(cls, *cls.mStruct);
  }
}

void CppFunctionPlugin::ProcessStructDefinition(Class &cls,
                                                const StructDefinition &structDefinition) {
  for (auto &mixin: structDefinition.mMixins) {
    ProcessStructDefinition(cls, *mProject.mProject.GetStruct(mixin));
  }
  for (auto &func: structDefinition.mFunctions) {
    if (!func.GetAnnotation(Annotations::LuaFunc))
      AddCppFunction(cls, func);
  }
}

void CppFunctionPlugin::AddCppFunction(Class &cls, const FunctionDefinition &functionDefinition) {
  auto method = NewFunction(functionDefinition);
  method.mUserDefined = true;
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
