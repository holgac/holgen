#include "CSharpWrapperGenerator.h"

#include "core/St.h"

#include "core/Annotations.h"
#include "../lang/CSharpHelper.h"

namespace holgen {
void CSharpWrapperGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (mGeneratorSettings.mCSharpRoot.empty())
    return;
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!ShouldProcess(cls))
      continue;
    Generate(contents.emplace_back(), cls);
  }
}

void CSharpWrapperGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  out.mType = FileType::CSharpSource;
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

  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::Singleton)) {
    return false;
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
    fieldsStruct.Add(
        "public {} {};",
        CSharpHelper::Get().RepresentationInNative(field.mType, field.mType, mTranslatedProject),
        mNamingConvention.FieldNameInCSharp(field.mField->mName));
    accessors.Add(
        "public {0} {1} => {2}.{1};",
        CSharpHelper::Get().RepresentationInNative(field.mType, field.mType, mTranslatedProject),
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

  if (cls.mStruct && cls.mStruct->GetAnnotation(Annotations::Singleton)) {
    return false;
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
                ConstructMethodSignatureArguments(
                    cls, ctor, ctor.mArguments, InteropType::NativeToManaged, false, false, true));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  auto caller = ConstructWrapperCall(cls, ctor, ctor.mFunction->mName, false);
  codeBlock.Add("{} = {};", St::CSharpMirroredStructFieldName, caller);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

bool CSharpWrapperGenerator::GenerateMethods(CodeBlock &codeBlock, const Class &cls) const {
  bool processed = false;
  for (auto &method: cls.mCFunctions) {
    if (!ShouldProcess(method))
      continue;
    processed = true;
    if (!method.mFunction ||
        !method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_Constructor))
      GenerateMethod(codeBlock, cls, method);
  }
  return processed;
}

bool CSharpWrapperGenerator::GenerateMethodDelegates(CodeBlock &codeBlock, const Class &cls) const {
  bool processed = false;
  for (auto &method: cls.mCFunctions) {
    if (!ShouldProcess(method))
      continue;
    processed = true;
    bool isStatic = method.mMethod->IsStatic(cls);
    GenerateMethodDelegate(codeBlock, cls, method, isStatic, !isStatic);
  }
  return processed;
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

bool CSharpWrapperGenerator::ShouldProcess(const Class &cls) const {
  if (!cls.mStruct) {
    return cls.mName == "DeferredDeleter";
  }
  if (cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
      cls.mStruct->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp) ||
      cls.mStruct->GetAnnotation(Annotations::DotNetModule))
    return false;
  return true;
}

bool CSharpWrapperGenerator::ShouldProcess(const CFunction &func) const {
  if (!func.mMethod)
    return false;
  return ShouldProcess(*func.mMethod);
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
