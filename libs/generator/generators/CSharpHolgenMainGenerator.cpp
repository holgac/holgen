#include "CSharpHolgenMainGenerator.h"

#include "core/St.h"
#include <filesystem>

namespace holgen {
void CSharpHolgenMainGenerator::Run(std::vector<GeneratedContent> &contents) const {
  if (mGeneratorSettings.mCSharpRoot.empty())
    return;
  auto &file = contents.emplace_back();
  file.mType = FileType::CSharpFile;
  file.mName = std::format("{}/HolgenMain.cs", St::CSharpProjectName);
  CodeBlock codeBlock;
  codeBlock.Add("using System.Runtime.InteropServices;");
  codeBlock.Add("using System.Reflection;");
  codeBlock.Add("");
  codeBlock.Add("namespace {};", St::CSharpProjectName);
  codeBlock.Add("");
  codeBlock.Add("public sealed class HolgenMain");
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("public static HolgenMain Instance = null!;");
  codeBlock.Add("public List<Assembly> Assemblies {{ get; }} = [];");
  codeBlock.Add("");

  codeBlock.Add("public HolgenMain()");
  codeBlock.Add("{{");
  codeBlock.Add("}}");
  codeBlock.Add("");

  codeBlock.Add("public static void Main()");
  codeBlock.Add("{{");
  codeBlock.Add("}}");
  codeBlock.Add("");

  codeBlock.Add("public delegate void InitializeDelegate();");
  codeBlock.Add("public static void Initialize()");
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("Instance = new HolgenMain();");
  codeBlock.Add("Console.WriteLine(\"Holgen C# initialization successful!\");");
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  codeBlock.Add("");

  codeBlock.Add("public delegate void LoadModuleDelegate([MarshalAs(UnmanagedType.LPStr)] "
                "string path);");
  codeBlock.Add("public static void LoadModule([MarshalAs(UnmanagedType.LPStr)] string path)");
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("Console.WriteLine(\"Loading module {{0}}\", path);");
  codeBlock.Add("var asm = Assembly.LoadFrom(path);");
  codeBlock.Add("Instance.Assemblies.Add(asm);");
  codeBlock.Indent(-1);
  codeBlock.Add("}}");

  codeBlock.Indent(-1);
  codeBlock.Add("}}");
  file.mBody = std::move(codeBlock);
}
} // namespace holgen
