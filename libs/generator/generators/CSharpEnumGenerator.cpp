#include "CSharpEnumGenerator.h"
#include "core/St.h"

#include <generator/utils/CSharpHelper.h>

namespace holgen {
void CSharpEnumGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (!mGeneratorSettings.IsFeatureEnabled(GeneratorFeatureFlag::CSharp))
    return;
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!cls.mEnum)
      continue;
    auto nestedEnum = cls.GetNestedEnum("Entry");
    THROW_IF(!nestedEnum, "Entry enum was not created properly for {}!", cls.mName);
    Process(contents.emplace_back(), cls, *nestedEnum);
  }
}

void CSharpEnumGenerator::Process(GeneratedContent &out, const Class &cls,
                                  const ClassEnum &clsEnum) const {
  out.mType = FileType::CSharpSource;
  out.mName = std::format("{}/{}.cs", St::CSharpProjectName, cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", St::GenMessage);
  codeBlock.Add("");
  codeBlock.Add("namespace {};", St::CSharpProjectName);
  codeBlock.Add("");
  if (cls.mEnum->mType == EnumDefinitionType::Bitmap)
    codeBlock.Add("[Flags]");
  std::string typeSpecifier;
  AddCppComments(codeBlock, cls.mComments);
  AddCppComments(codeBlock, clsEnum.mComments);
  if (!clsEnum.mUnderlyingType.empty()) {
    auto csType = CSharpHelper::Get().ConvertFieldType(
        Type{clsEnum.GetUnderlyingType(cls)}, mTranslatedProject, InteropType::Internal, false);
    codeBlock.Add("public enum {} : {}", cls.mName, csType.mName);
  } else {
    codeBlock.Add("public enum {}", cls.mName);
  }
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  for (auto &entry: clsEnum.mEntries) {
    AddCppComments(codeBlock, entry.mComments);
    codeBlock.Add("{} = {},", entry.mName, entry.mValue);
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  out.mBody = std::move(codeBlock);
}
} // namespace holgen
