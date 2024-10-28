#include "CSharpWrapperGenerator.h"

#include "core/St.h"

#include "core/Annotations.h"
#include "../lang/CSharpHelper.h"

namespace holgen {
void CSharpWrapperGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (mGeneratorSettings.mCSharpRoot.empty())
    return;
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!cls.mStruct)
      continue;
    if (cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
        cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp) ||
        cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    Generate(contents.emplace_back(), cls);
  }
}

void CSharpWrapperGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  out.mType = FileType::CSharpFile;
  out.mName = std::format("{}/{}.cs", St::CSharpProjectName, cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("namespace {};", St::CSharpProjectName);
  codeBlock.Add("");
  codeBlock.Add("using System.Runtime.InteropServices;");
  codeBlock.Add("");
  codeBlock.Add("public class {}", cls.mName);
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  GenerateClassBody(codeBlock, cls);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  out.mBody = std::move(codeBlock);
}

void CSharpWrapperGenerator::GenerateClassBody(CodeBlock &codeBlock, const Class &cls) const {
  if (GenerateConstructors(codeBlock, cls))
    codeBlock.Add("");
  if (GenerateFields(codeBlock, cls))
    codeBlock.Add("");
  if (GenerateMethods(codeBlock, cls))
    codeBlock.Add("");
  if (GenerateMethodDelegates(codeBlock, cls))
    codeBlock.Add("");
  if (GenerateMethodPointers(codeBlock, cls))
    codeBlock.Add("");
  GenerateInitializerDelegate(codeBlock, cls);
  codeBlock.Add("");
  GenerateInitializer(codeBlock, cls);
}

bool CSharpWrapperGenerator::GenerateFields(CodeBlock &codeBlock, const Class &cls) const {
  if (cls.IsProxyable()) {
    codeBlock.Add("public IntPtr {} {{ get; }}", St::CSharpProxyObjectPointerFieldName);
    return true;
  }
  bool processed = false;
  CodeBlock fieldsStruct;
  CodeBlock accessors;
  fieldsStruct.Add("[StructLayout(LayoutKind.Sequential)]");
  fieldsStruct.Add("public struct {}", St::CSharpMirroredStructStructName);
  fieldsStruct.Add("{{");
  fieldsStruct.Indent(1);
  for (auto &field: cls.mFields) {
    if (!ShouldProcess(field))
      continue;
    processed = true;
    fieldsStruct.Add("public {} {};",
                     CSharpHelper::Get().RepresentationInNative(field.mType, mTranslatedProject),
                     mNamingConvention.FieldNameInCSharp(field.mField->mName));
    accessors.Add("public {0} {1} => {2}.{1};",
                  CSharpHelper::Get().RepresentationInNative(field.mType, mTranslatedProject),
                  mNamingConvention.FieldNameInCSharp(field.mField->mName),
                  St::CSharpMirroredStructFieldName);
  }

  if (processed) {
    fieldsStruct.Indent(-1);
    fieldsStruct.Add("}}");
    codeBlock.Add(std::move(fieldsStruct));
    codeBlock.Add("");
    codeBlock.Add("public {} {};", St::CSharpMirroredStructStructName,
                  St::CSharpMirroredStructFieldName);
    codeBlock.Add(std::move(accessors));
  }
  return processed;
}

void CSharpWrapperGenerator::GenerateInitializerDelegate(CodeBlock &codeBlock,
                                                         const Class &cls) const {
  codeBlock.Add(
      "public delegate void {}({});",
      mNamingConvention.CSharpMethodDelegateName(cls.mName, St::CSharpInitializerMethodName),
      ConstructInitializerArguments(cls));
}

bool CSharpWrapperGenerator::GenerateConstructors(CodeBlock &codeBlock, const Class &cls) const {
  if (cls.IsProxyable()) {
    GenerateProxyConstructor(codeBlock, cls);
    return true;
  }

  bool processed = false;
  for (auto &ctor: cls.mConstructors) {
    if (!ShouldProcess(ctor))
      continue;
    processed = true;
    GenerateConstructor(codeBlock, cls, ctor);
  }
  if (processed) {
    GenerateEmptyConstructor(codeBlock, cls);
  }
  return processed;
}

void CSharpWrapperGenerator::GenerateProxyConstructor(CodeBlock &codeBlock,
                                                      const Class &cls) const {
  codeBlock.Add("public {}(IntPtr ptr)", cls.mName);
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("{} = ptr;", St::CSharpProxyObjectPointerFieldName);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void CSharpWrapperGenerator::GenerateEmptyConstructor(CodeBlock &codeBlock,
                                                      const Class &cls) const {
  codeBlock.Add("public {}()", cls.mName);
  codeBlock.Add("{{");
  codeBlock.Add("}}");
}

void CSharpWrapperGenerator::GenerateConstructor(CodeBlock &codeBlock, const Class &cls,
                                                 const ClassConstructor &ctor) const {
  codeBlock.Add("public {}({})", cls.mName,
                ConstructMethodSignatureArguments(cls, ctor, InteropType::NativeToManaged, false));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  auto caller = ConstructWrapperCall(cls, ctor, ctor.mFunction->mName, false);
  codeBlock.Add("{} = {};", St::CSharpMirroredStructFieldName, caller);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

bool CSharpWrapperGenerator::GenerateMethods(CodeBlock &codeBlock, const Class &cls) const {
  bool processed = false;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    processed = true;
    if (!method.mFunction ||
        !method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_Constructor))
      GenerateMethod(codeBlock, cls, method);
  }
  return processed;
}

void CSharpWrapperGenerator::GenerateMethod(CodeBlock &codeBlock, const Class &cls,
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

bool CSharpWrapperGenerator::GenerateMethodDelegates(CodeBlock &codeBlock, const Class &cls) const {
  bool processed = false;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    processed = true;
    GenerateMethodDelegate(codeBlock, cls, method);
  }
  return processed;
}

void CSharpWrapperGenerator::GenerateMethodDelegate(CodeBlock &codeBlock, const Class &cls,
                                                    const ClassMethod &method) const {
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
  codeBlock.Add("public delegate {} {}({});",
                CSharpHelper::Get().RepresentationInNative(method.mReturnType, mTranslatedProject),
                mNamingConvention.CSharpMethodDelegateName(cls, method),
                ConstructMethodSignatureArguments(cls, method, InteropType::ManagedToNative,
                                                  !method.IsStatic(cls)));
}

bool CSharpWrapperGenerator::GenerateMethodPointers(CodeBlock &codeBlock, const Class &cls) const {
  bool processed = false;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    processed = true;
    GenerateMethodPointer(codeBlock, method);
  }
  return processed;
}

void CSharpWrapperGenerator::GenerateMethodPointer(CodeBlock &codeBlock,
                                                   const ClassMethod &method) const {
  codeBlock.Add("private static IntPtr {} = IntPtr.Zero;",
                mNamingConvention.CSharpMethodPointerName(method));
}

bool CSharpWrapperGenerator::ShouldProcess(const ClassMethod &method) const {
  if (!method.mExposeToScript)
    return false;
  if (method.mFunction &&
      (method.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
       method.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp)))
    return false;
  return true;
}

bool CSharpWrapperGenerator::ShouldProcess(const ClassConstructor &ctor) const {
  if (!ctor.mFunction ||
      ctor.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      ctor.mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp))
    return false;
  return true;
}

bool CSharpWrapperGenerator::ShouldProcess(const ClassField &field) const {
  if (!field.mField)
    return false;
  if (field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      field.mField->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp))
    return false;
  return true;
}

std::string CSharpWrapperGenerator::ConstructInitializerArguments(const Class &cls) const {
  std::stringstream ss;
  bool isFirst = true;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    ss << std::format("{} {}", mNamingConvention.CSharpMethodDelegateName(cls, method),
                      St::Uncapitalize(method.mName));
  }
  return ss.str();
}

std::string CSharpWrapperGenerator::ConstructMethodSignatureArguments(const Class &cls,
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
        "{} {}",
        CSharpHelper::Get().Representation(arg.mType, mTranslatedProject, interopType, true),
        arg.mName);
  }
  return ss.str();
}

std::string CSharpWrapperGenerator::ConstructMethodArguments(const Class &cls,
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

std::string CSharpWrapperGenerator::ConstructWrapperCall(const Class &cls, const MethodBase &method,
                                                         const std::string &methodName,
                                                         bool addThisArgument) const {
  return std::format(
      "Marshal.GetDelegateForFunctionPointer<{}>({})({})",
      mNamingConvention.CSharpMethodDelegateName(cls.mName, methodName),
      mNamingConvention.CSharpMethodPointerName(methodName),
      ConstructMethodArguments(cls, method, InteropType::ManagedToNative, addThisArgument));
}

void CSharpWrapperGenerator::GenerateInitializer(CodeBlock &codeBlock, const Class &cls) const {
  codeBlock.Add("public static void {}({})", St::CSharpInitializerMethodName,
                ConstructInitializerArguments(cls));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method)) {
      continue;
    }
    codeBlock.Add("{} = Marshal.GetFunctionPointerForDelegate({});",
                  mNamingConvention.CSharpMethodPointerName(method),
                  St::Uncapitalize(method.mName));
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}
} // namespace holgen
