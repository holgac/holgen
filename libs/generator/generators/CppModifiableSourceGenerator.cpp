#include "CppModifiableSourceGenerator.h"
#include "core/St.h"

#include "core/Annotations.h"

namespace holgen {
void CppModifiableSourceGenerator::Run(std::vector<GeneratedContent> &contents) const {
  for (auto &cls: mTranslatedProject.mClasses) {
    if (!cls.HasUserDefinedMethods())
      continue;
    Generate(contents.emplace_back(), cls);
  }
}

void CppModifiableSourceGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  out.mType = FileType::CppSource;
  out.mName = std::format("src/{}.cpp", cls.mName);
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
    std::string sectionSuffix;
    if (method.mFunction &&
        method.mFunction->GetMatchingAttribute(Annotations::Func,
                                               Annotations::Func_OverloadSuffix)) {
      sectionSuffix =
          method.mFunction->GetMatchingAttribute(Annotations::Func, Annotations::Func_OverloadSuffix)
              ->mValue.mName;
    }
    codeBlock.UserDefined("{}_{}{}{}", cls.mName, method.mName, sectionSuffix,
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
  out.mBody = std::move(codeBlock);
}
} // namespace holgen
