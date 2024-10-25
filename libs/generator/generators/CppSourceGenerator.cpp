#include "CppSourceGenerator.h"
#include "core/St.h"

namespace holgen {
void CppSourceGenerator::Run(std::vector<GeneratedContent> &contents) const {
  for (auto &cls: mTranslatedProject.mClasses) {
    Generate(contents.emplace_back(), cls);
  }
}

void CppSourceGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  auto headerHeaders = PrepareIncludes(cls, true);
  auto headers = PrepareIncludes(cls, false);
  headers.Subtract(headerHeaders);

  out.mType = FileType::CppSource;
  out.mName = std::format("gen/{}.cpp", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", St::GenMessage);
  codeBlock.Add("#include \"{}.h\"", cls.mName);
  codeBlock.Line();
  headers.Write(codeBlock);

  if (!cls.mNamespace.empty())
    codeBlock.Add("namespace {} {{", cls.mNamespace);

  bool previousBlockWasEmpty = true;
  {
    auto block = GenerateFieldsForSource(cls);
    codeBlock.Add(std::move(block));
    previousBlockWasEmpty = block.mContents.empty();
  }

  {
    auto block = GenerateConstructorsForSource(cls);
    if (!block.mContents.empty()) {
      if (!previousBlockWasEmpty)
        codeBlock.AddLine();
      codeBlock.Add(std::move(block));
      previousBlockWasEmpty = false;
    }
  }

  {
    auto block = GenerateDestructor(cls);
    if (!block.mContents.empty()) {
      if (!previousBlockWasEmpty)
        codeBlock.AddLine();
      codeBlock.Add(std::move(block));
    }
  }

  {
    auto block = GenerateMethodsForSource(cls);
    if (!block.mContents.empty()) {
      if (!previousBlockWasEmpty)
        codeBlock.AddLine();
      codeBlock.Add(std::move(block));
    }
  }

  if (!cls.mNamespace.empty())
    codeBlock.Add("}}"); // namespace

  GenerateCFunctionsForSource(codeBlock, cls);

  out.mBody = std::move(codeBlock);
}

CodeBlock CppSourceGenerator::GenerateConstructorsForSource(const Class &cls) const {
  CodeBlock codeBlock;
  if (!cls.mTemplateParameters.empty())
    return {};
  bool isFirstMethod = true;

  for (auto &ctor: cls.mConstructors) {
    if (!ctor.mTemplateParameters.empty() || ctor.mDefaultDelete != DefaultDelete::Neither) {
      // These are defined in the header
      continue;
    }

    if (ctor.mUserDefined) {
      continue;
    }

    if (isFirstMethod)
      isFirstMethod = false;
    else
      codeBlock.AddLine();
    if (ctor.mIsTemplateSpecialization) {
      codeBlock.Add("template <>");
    }
    {
      auto line = codeBlock.Line();
      line << GenerateFunctionSignature(cls, ctor, false, false);
      if (ctor.mInitializerList.empty())
        line << " {";
      else
        line << " :";
    }
    if (!ctor.mInitializerList.empty()) {
      codeBlock.Indent(2);
      {
        auto line = codeBlock.Line();
        bool isFirst = true;
        for (auto &iElem: ctor.mInitializerList) {
          if (isFirst)
            isFirst = false;
          else
            line << ", ";
          line << iElem.mDestination << "(" << iElem.mValue << ")";
        }
        line << " {";
      }
      codeBlock.Indent(-2);
    }
    codeBlock.Indent(1);
    codeBlock.Add(ctor.mBody);
    codeBlock.Indent(-1);
    codeBlock.Line() << "}";
  }
  return codeBlock;
}

CodeBlock CppSourceGenerator::GenerateFieldsForSource(const Class &cls) const {
  CodeBlock codeBlock;
  for (auto &field: cls.mFields) {
    if (field.mStaticness == Staticness::Static && field.mType.mName == cls.mName) {
      codeBlock.Add("{}{}::{}{};", field.mType.ToString(false), cls.mName, field.mName,
                    StringifyFieldDefinition(field));
    }
  }
  return codeBlock;
}

CodeBlock CppSourceGenerator::GenerateMethodsForSource(const Class &cls) const {
  CodeBlock codeBlock;
  if (!cls.mTemplateParameters.empty())
    return {};

  bool isFirstMethod = true;

  for (auto &method: cls.mMethods) {
    if (CanBeDefinedInHeader(cls, method) || method.mUserDefined) {
      continue;
    }
    if (isFirstMethod)
      isFirstMethod = false;
    else
      codeBlock.AddLine();
    if (method.mIsTemplateSpecialization) {
      codeBlock.Add("template <>");
    }
    codeBlock.Line() << GenerateFunctionSignature(cls, method, false, false) << " {";
    codeBlock.Indent(1);
    codeBlock.Add(method.mBody);
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
  }
  return codeBlock;
}

void CppSourceGenerator::GenerateCFunctionsForSource(CodeBlock &codeBlock, const Class &cls) const {
  if (cls.mCFunctions.empty())
    return;
  codeBlock.Add("extern \"C\" {{");
  codeBlock.Indent(1);
  for (auto &func: cls.mCFunctions) {
    codeBlock.Add("{} {{", GenerateFunctionSignature(func, false));
    codeBlock.Indent(1);
    codeBlock.Add(func.mBody);
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}
} // namespace holgen
