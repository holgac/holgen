#include "CSharpProjectGenerator.h"

#include "core/St.h"
#include <filesystem>

namespace holgen {
void CSharpProjectGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (mGeneratorSettings.mCSharpRoot.empty())
    return;
  auto &proj = contents.emplace_back();
  proj.mType = FileType::CSharpProject;
  proj.mName = std::format("{0}/{0}.csproj", St::CSharpProjectName);
  CodeBlock codeBlock;
  codeBlock.Add("<Project Sdk=\"Microsoft.NET.Sdk\">");
  codeBlock.Indent(1);
  codeBlock.Add("<PropertyGroup>");
  codeBlock.Indent(1);
  codeBlock.Add("<TargetFramework>net8.0</TargetFramework>");
  codeBlock.Add("<ImplicitUsings>enable</ImplicitUsings>");
  codeBlock.Add("<Nullable>enable</Nullable>");
  codeBlock.Add("<AllowUnsafeBlocks>true</AllowUnsafeBlocks>");
  codeBlock.Add("<OutputType>Exe</OutputType>");
  codeBlock.Add("<LangVersion>13</LangVersion>");
  codeBlock.Indent(-1);
  codeBlock.Add("</PropertyGroup>");
  codeBlock.Indent(-1);
  codeBlock.Add("</Project>");
  proj.mBody = std::move(codeBlock);
}
} // namespace holgen
