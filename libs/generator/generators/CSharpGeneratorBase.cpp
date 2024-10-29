#include "CSharpGeneratorBase.h"

#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void CSharpGeneratorBase::GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls,
                                                 const CFunction &func, bool isStatic,
                                                 bool ignoreFirstArg) const {
  codeBlock.Add("public delegate {} {}({});",
                CSharpHelper::Get().RepresentationInNative(
                    func.mReturnType, func.mMethod ? func.mMethod->mReturnType : func.mReturnType,
                    mTranslatedProject),
                mNamingConvention.CSharpMethodDelegateName(cls.mName, func.mMethod->mName),
                ConstructMethodSignatureArguments(
                    cls, func, func.mMethod ? func.mMethod->mArguments : func.mArguments,
                    InteropType::ManagedToNative, !isStatic, ignoreFirstArg, false));
}

void CSharpGeneratorBase::GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls,
                                                 const ClassMethod &method, bool isStatic,
                                                 bool ignoreFirstArg) const {
  codeBlock.Add("public delegate {} {}({});",
                CSharpHelper::Get().RepresentationInNative(method.mReturnType, method.mReturnType,
                                                           mTranslatedProject),
                mNamingConvention.CSharpMethodDelegateName(cls.mName, method.mName),
                ConstructMethodSignatureArguments(cls, method, method.mArguments,
                                                  InteropType::ManagedToNative, !isStatic,
                                                  ignoreFirstArg, false));
}

std::string CSharpGeneratorBase::ConstructMethodSignatureArguments(
    const Class &cls, const MethodBase &method, const std::list<MethodArgument> &originalArguments,
    InteropType interopType, bool addThisArgument, bool ignoreFirstArg,
    bool ignoreAuxiliaries) const {
  std::stringstream ss;
  bool isFirst = true;
  if (addThisArgument) {
    isFirst = false;
    ss << CSharpHelper::Get().Representation(Type{cls.mName}, Type{cls.mName}, mTranslatedProject,
                                             interopType, true)
       << " holgenObject";
  }
  std::ptrdiff_t argIdx = -1;
  std::map<std::string, const MethodArgument *> origArgMap;
  for (auto &origArg: originalArguments) {
    origArgMap.emplace(origArg.mName, &origArg);
  }

  for (auto it = method.mArguments.begin(), end = method.mArguments.end(); it != end; ++it) {
    ++argIdx;
    if (argIdx == 0 && ignoreFirstArg) {
      continue;
    }
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    auto nextIt = it;
    ++nextIt;
    auto origIt = origArgMap.find(it->mName);
    const MethodArgument *origArg = origIt == origArgMap.end() ? &(*it) : origIt->second;
    if (nextIt != method.mArguments.end() &&
        it->mName + St::CSharpAuxiliarySizeSuffix == nextIt->mName) {
      ss << std::format("{}{} {}",
                        CSharpHelper::Get().ArrayMarshallingInfo(it->mType, mTranslatedProject,
                                                                 interopType, argIdx + 1),
                        CSharpHelper::Get().Representation(it->mType, origArg->mType,
                                                           mTranslatedProject, interopType, true),
                        it->mName);
      if (ignoreAuxiliaries)
        ++it;
    } else {
      ss << std::format(
          "{}{} {}",
          CSharpHelper::Get().MarshallingInfo(it->mType, mTranslatedProject, interopType),
          CSharpHelper::Get().Representation(it->mType, origArg->mType, mTranslatedProject,
                                             interopType, true),
          it->mName);
    }
  }
  return ss.str();
}

void CSharpGeneratorBase::GenerateAbstractMethod(CodeBlock &codeBlock, const Class &cls,
                                                 const ClassMethod &method, bool isStatic,
                                                 InteropType interopType) const {
  codeBlock.Add("public{} abstract {} {}({});", isStatic ? " static" : "",
                CSharpHelper::Get().Representation(method.mReturnType, method.mReturnType,
                                                   mTranslatedProject, interopType, false),
                method.mName,
                ConstructMethodSignatureArguments(cls, method, method.mArguments, interopType,
                                                  false, false, false));
}

void CSharpGeneratorBase::GenerateMethod(CodeBlock &codeBlock, const Class &cls,
                                         const CFunction &func) const {
  bool isStatic = func.mMethod->IsStatic(cls);
  bool returnsVoid = func.mReturnType.mName == "void";
  codeBlock.Add("public{} {} {}({})", isStatic ? " static" : "",
                CSharpHelper::Get().Representation(
                    func.mReturnType, func.mMethod ? func.mMethod->mReturnType : func.mReturnType,
                    mTranslatedProject, InteropType::NativeToManaged, false),
                func.mMethod->mName,
                ConstructMethodSignatureArguments(
                    cls, func, func.mMethod ? func.mMethod->mArguments : func.mArguments,
                    InteropType::NativeToManaged, false, !isStatic, true));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  auto caller = ConstructWrapperCall(cls, func, func.mMethod->mName, !isStatic);
  if (returnsVoid) {
    codeBlock.Add("{};", caller);
  } else if (auto retClass = mTranslatedProject.GetClass(func.mReturnType.mName)) {
    if (retClass->mStruct &&
        retClass->mStruct->GetMatchingAttribute(Annotations::Script,
                                                Annotations::Script_AlwaysMirror)) {
      codeBlock.Add("return new {}", func.mReturnType.mName);
      codeBlock.Add("{{");
      codeBlock.Indent(1);
      codeBlock.Add("{} = {}", St::CSharpMirroredStructFieldName, caller);
      codeBlock.Indent(-1);
      codeBlock.Add("}};");
    } else {
      codeBlock.Add("return new {}({});", func.mReturnType.mName, caller);
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
  for (auto it = method.mArguments.begin(), end = method.mArguments.end(); it != end; ++it) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    if (addThisArgument) {
      ss << CSharpHelper::Get().VariableRepresentation(Type{cls.mName}, "this", mTranslatedProject,
                                                       interopType, true);
      addThisArgument = false;
      continue;
    }
    if (it != method.mArguments.begin()) {
      auto prevIt = it;
      --prevIt;
      if (prevIt->mName + St::CSharpAuxiliarySizeSuffix == it->mName) {
        auto variableName = std::format("(ulong){}.Length", prevIt->mName);
        ss << CSharpHelper::Get().VariableRepresentation(it->mType, variableName,
                                                         mTranslatedProject, interopType, true);
        continue;
      }
    }
    ss << CSharpHelper::Get().VariableRepresentation(it->mType, it->mName, mTranslatedProject,
                                                     interopType, true);
  }
  return ss.str();
}

} // namespace holgen
