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
  auto method = NewFunction(cls, functionDefinition);
  if (functionDefinition.GetMatchingAttribute(Annotations::Func, Annotations::Func_Static)) {
    method.mStaticness = Staticness::Static;
  }
  if (functionDefinition.GetMatchingAttribute(Annotations::Func, Annotations::Func_Noexcept)) {
    method.mNoexceptness = Noexceptness::Noexcept;
  }
  method.mUserDefined = true;
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
  if (functionDefinition.GetMatchingAttribute(Annotations::Func, Annotations::Func_Constructor)) {
    ProcessConstructorAttribute(cls, cls.mMethods.back());
  }
}

void CppFunctionPlugin::ProcessConstructorAttribute(Class &cls, ClassMethod &method) {
  AddCppConstructor(cls, *method.mFunction);

  method.mStaticness = Staticness::Static;
  method.mUserDefined = false;
  {
    auto line = method.mBody.Line();
    line << "return " << cls.mName << "(";
    bool isFirst = true;
    for (auto &arg: method.mArguments) {
      if (isFirst) {
        isFirst = false;
      } else {
        line << ", ";
      }
      line << arg.mName;
    }
    line << ");";
  }
  method.mReturnType = Type{cls.mName};
}

void CppFunctionPlugin::AddCppConstructor(Class &cls,
                                          const FunctionDefinition &functionDefinition) {
  auto ctor = NewConstructor(cls, functionDefinition);
  ctor.mUserDefined = true;
  if (functionDefinition.mArguments.size() == 1)
    ctor.mExplicitness = Explicitness::Explicit;
  cls.mConstructors.push_back(std::move(ctor));
}
} // namespace holgen
