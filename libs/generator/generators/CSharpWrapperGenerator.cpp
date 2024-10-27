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
  if (!cls.mStruct->GetMatchingAttribute(Annotations::Script, Annotations::Script_AlwaysMirror))
    return false;
  bool processed = false;
  CodeBlock tempCodeBlock;
  tempCodeBlock.Add("[StructLayout(LayoutKind.Sequential)]");
  tempCodeBlock.Add("public struct Fields");
  tempCodeBlock.Add("{{");
  tempCodeBlock.Indent(1);
  for (auto &field: cls.mFields) {
    if (!ShouldProcess(field))
      continue;
    processed = true;
    tempCodeBlock.Add("public {} {};", CSharpHelper::Get().TypeRepresentation(field.mType, mTranslatedProject),
                      mNamingConvention.FieldNameInCSharp(field.mField->mName));
  }

  if (processed) {
    tempCodeBlock.Indent(-1);
    tempCodeBlock.Add("}}");
    codeBlock.Add(std::move(tempCodeBlock));
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

bool CSharpWrapperGenerator::GenerateMethods(CodeBlock &codeBlock, const Class &cls) const {
  bool processed = false;
  for (auto &method: cls.mMethods) {
    if (!ShouldProcess(method))
      continue;
    processed = true;
    GenerateMethod(codeBlock, cls, method);
  }
  return processed;
}

void CSharpWrapperGenerator::GenerateMethod(CodeBlock &codeBlock, const Class &cls,
                                            const ClassMethod &method) const {
  bool isStatic = method.IsStatic(cls);
  bool returnsVoid = method.mReturnType.mName == "void";
  std::string staticnessString = isStatic ? " static" : "";
  codeBlock.Add("public{} {} {}({})", isStatic ? " static" : "",
                CSharpHelper::Get().TypeRepresentation(method.mReturnType, mTranslatedProject), method.mName,
                ConstructMethodSignatureArguments(method));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("{}Marshal.GetDelegateForFunctionPointer<{}>({})({});",
                returnsVoid ? "" : "var holgenResultValue = ",
                mNamingConvention.CSharpMethodDelegateName(cls, method),
                mNamingConvention.CSharpMethodPointerName(method),
                ConstructMethodArguments(method));
  if (!returnsVoid) {
    codeBlock.Add("return holgenResultValue;");
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
    args << CSharpHelper::Get().TypeRepresentation(arg.mType, mTranslatedProject) << " " << arg.mName;
  }
  codeBlock.Add("public delegate {} {}({});", CSharpHelper::Get().TypeRepresentation(method.mReturnType, mTranslatedProject),
                mNamingConvention.CSharpMethodDelegateName(cls, method), args.str());
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

std::string
    CSharpWrapperGenerator::ConstructMethodSignatureArguments(const ClassMethod &method) const {
  std::stringstream ss;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    ss << std::format("{} {}", CSharpHelper::Get().TypeRepresentation(arg.mType, mTranslatedProject), arg.mName);
  }
  return ss.str();
}

std::string CSharpWrapperGenerator::ConstructMethodArguments(const ClassMethod &method) const {
  std::stringstream ss;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    ss << arg.mName;
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
