#include "CPPModifiableSourceGenerator.h"
#include "core/St.h"

namespace holgen {
void CPPModifiableSourceGenerator::Run(std::vector<GeneratedContent> &contents) {
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!cls.HasUserDefinedMethods())
      continue;
    Generate(contents.emplace_back(), cls);
  }
}

void CPPModifiableSourceGenerator::Generate(GeneratedContent &source, const Class &cls) {
  source.mType = FileType::CppSource;
  source.mName = std::format("src/{}.cpp", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", St::PartialGenMessage);
  codeBlock.Add("#include \"../gen/{}.h\"", cls.mName);
  codeBlock.UserDefined("{}_CustomIncludes", cls.mName);
  codeBlock.Line();
  if (!cls.mNamespace.empty())
    codeBlock.Add("namespace {} {{", cls.mNamespace);
  for (auto &method: cls.mMethods) {
    if (!method.mUserDefined || method.mVirtuality == Virtuality::PureVirtual)
      continue;
    codeBlock.Add("{} {{", GenerateFunctionSignature(cls, method, false, false));
    codeBlock.UserDefined("{}_{}{}", cls.mName, method.mName,
                          method.mConstness == Constness::Const ? "_Const" : "");
    codeBlock.Add("}}");
    codeBlock.AddLine();
  }

  for (auto &ctor: cls.mConstructors) {
    if (!ctor.mUserDefined)
      continue;
    codeBlock.AddLine(GenerateFunctionSignature(cls, ctor, false, false));
    codeBlock.UserDefined("INITIALIZER_{}_{}", cls.mName, ctor.mFunction->mName);
    codeBlock.Add("{{");
    codeBlock.UserDefined("{}_{}", cls.mName, ctor.mFunction->mName);
    codeBlock.Add("}}");
    codeBlock.AddLine();
  }
  if (!cls.mNamespace.empty())
    codeBlock.Add("}}");
  source.mBody = std::move(codeBlock);
}
} // namespace holgen
