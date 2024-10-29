#include "CSharpModuleInterfaceGenerator.h"
#include <filesystem>

#include "core/St.h"
#include "core/Annotations.h"

namespace holgen {
void CSharpModuleInterfaceGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (mGeneratorSettings.mCSharpRoot.empty())
    return;
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    Generate(contents.emplace_back(), cls);
  }
}

void CSharpModuleInterfaceGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  out.mType = FileType::CSharpSource;
  out.mName = std::format("{}/{}.cs", St::CSharpProjectName,
                          mNamingConvention.ModuleInterfaceNameInCSharp(cls.mName));
  CodeBlock codeBlock;

  codeBlock.Add("using System.Runtime.InteropServices;");
  codeBlock.Add("");
  codeBlock.Add("namespace {};", St::CSharpProjectName);
  codeBlock.Add("");
  codeBlock.Add("public interface {}", mNamingConvention.ModuleInterfaceNameInCSharp(cls.mName));
  codeBlock.Add("{{");
  codeBlock.Indent(1);

  for (auto &method: cls.mMethods) {
    GenerateAbstractMethod(codeBlock, cls, method, true, InteropType::ManagedToNative);
  }
  codeBlock.Add("");

  for (auto &method: cls.mMethods) {
    GenerateMethodDelegate(codeBlock, cls, method, true, false);
  }

  codeBlock.Indent(-1);
  codeBlock.Add("}}");

  out.mBody = std::move(codeBlock);
}
} // namespace holgen
