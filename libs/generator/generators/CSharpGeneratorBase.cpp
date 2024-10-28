#include "CSharpGeneratorBase.h"

#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void CSharpGeneratorBase::GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls,
                                                 const ClassMethod &method, bool isStatic) const {
  std::stringstream args;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      args << ", ";
    }
    args << CSharpHelper::Get().RepresentationInNative(arg.mType, mTranslatedProject) << " "
         << arg.mName;
  }
  codeBlock.Add(
      "public delegate {} {}({});",
      CSharpHelper::Get().RepresentationInNative(method.mReturnType, mTranslatedProject),
      mNamingConvention.CSharpMethodDelegateName(cls, method),
      ConstructMethodSignatureArguments(cls, method, InteropType::ManagedToNative, !isStatic));
}

std::string CSharpGeneratorBase::ConstructMethodSignatureArguments(const Class &cls,
                                                                   const MethodBase &method,
                                                                   InteropType interopType,
                                                                   bool addThisArgument) const {
  std::stringstream ss;
  bool isFirst = true;
  if (addThisArgument) {
    isFirst = false;
    ss << CSharpHelper::Get().Representation(Type{cls.mName}, mTranslatedProject, interopType, true)
       << " holgenObject";
  }
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    ss << std::format(
        "{}{} {}", CSharpHelper::Get().MarshallingInfo(arg.mType, mTranslatedProject, interopType),
        CSharpHelper::Get().Representation(arg.mType, mTranslatedProject, interopType, true),
        arg.mName);
  }
  return ss.str();
}

void CSharpGeneratorBase::GenerateAbstractMethod(CodeBlock &codeBlock, const Class &cls,
                                                 const ClassMethod &method, bool isStatic,
                                                 InteropType interopType) const {
  codeBlock.Add("public{} abstract {} {}({});", isStatic ? " static" : "",
                CSharpHelper::Get().Representation(method.mReturnType, mTranslatedProject,
                                                   interopType, false),
                method.mName, ConstructMethodSignatureArguments(cls, method, interopType, false));
}

void CSharpGeneratorBase::GenerateMethod(CodeBlock &codeBlock, const Class &cls,
                                         const ClassMethod &method) const {
  bool isStatic = method.IsStatic(cls);
  bool returnsVoid = method.mReturnType.mName == "void";
  codeBlock.Add(
      "public{} {} {}({})", isStatic ? " static" : "",
      CSharpHelper::Get().Representation(method.mReturnType, mTranslatedProject,
                                         InteropType::NativeToManaged, false),
      method.mName,
      ConstructMethodSignatureArguments(cls, method, InteropType::NativeToManaged, false));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  auto caller = ConstructWrapperCall(cls, method, method.mName, !isStatic);
  if (returnsVoid) {
    codeBlock.Add("{};", caller);
  } else if (auto retClass = mTranslatedProject.GetClass(method.mReturnType.mName)) {
    if (retClass->mStruct &&
        retClass->mStruct->GetMatchingAttribute(Annotations::Script,
                                                Annotations::Script_AlwaysMirror)) {
      codeBlock.Add("return new {}", method.mReturnType.mName);
      codeBlock.Add("{{");
      codeBlock.Indent(1);
      codeBlock.Add("{} = {}", St::CSharpMirroredStructFieldName, caller);
      codeBlock.Indent(-1);
      codeBlock.Add("}};");
    } else {
      codeBlock.Add("return new {}({});", method.mReturnType.mName, caller);
    }
  } else {
    codeBlock.Add("return {};", caller);
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

std::string CSharpGeneratorBase::ConstructWrapperCall(const Class &cls, const MethodBase &method,
                                                      const std::string &methodName,
                                                      bool addThisArgument) const {
  return std::format(
      "Marshal.GetDelegateForFunctionPointer<{}>({})({})",
      mNamingConvention.CSharpMethodDelegateName(cls.mName, methodName),
      mNamingConvention.CSharpMethodPointerName(methodName),
      ConstructMethodArguments(cls, method, InteropType::ManagedToNative, addThisArgument));
}

std::string CSharpGeneratorBase::ConstructMethodArguments(const Class &cls,
                                                          const MethodBase &method,
                                                          InteropType interopType,
                                                          bool addThisArgument) const {
  std::stringstream ss;
  bool isFirst = true;
  if (addThisArgument) {
    isFirst = false;
    ss << CSharpHelper::Get().VariableRepresentation(Type{cls.mName}, "this", mTranslatedProject,
                                                     interopType, true);
  }
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    ss << CSharpHelper::Get().VariableRepresentation(arg.mType, arg.mName, mTranslatedProject,
                                                     interopType, true);
  }
  return ss.str();
}

} // namespace holgen
