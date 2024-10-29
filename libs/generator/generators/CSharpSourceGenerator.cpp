#include "CSharpSourceGenerator.h"
#include "core/St.h"

namespace holgen {
void CSharpSourceGenerator::Run(std::vector<GeneratedContent> &contents) const {
  for (auto &csCls: mTranslatedProject.mCSharpClasses) {
    Process(contents.emplace_back(), csCls);
  }
}

void CSharpSourceGenerator::Process(GeneratedContent &out, const CSharpClass &csCls) const {
  out.mType = FileType::CSharpSource;
  out.mName = std::format("{}/{}.cs", St::CSharpProjectName, csCls.mName);
  CodeBlock codeBlock;
  GenerateUsingDirectives(codeBlock, csCls);
  GenerateClassNamespace(codeBlock);

  codeBlock.Add("{} {}{} {}", csCls.mVisibility,
                csCls.mStaticness == Staticness::Static ? "static " : "", csCls.mType, csCls.mName);
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  GenerateClassBody(codeBlock, csCls);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");

  out.mBody = std::move(codeBlock);
}

void CSharpSourceGenerator::GenerateUsingDirectives(CodeBlock &codeBlock,
                                                    const CSharpClass &csCls) const {
  for (auto &package: csCls.mUsingDirectives) {
    codeBlock.Add("using {};", package);
  }
  if (!csCls.mUsingDirectives.empty())
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateClassNamespace(CodeBlock &codeBlock) const {
  codeBlock.Add("namespace {};", St::CSharpProjectName);
  codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateClassBody(CodeBlock &codeBlock,
                                              const CSharpClass &csCls) const {
  GenerateMethods(codeBlock, csCls);
  GenerateDelegates(codeBlock, csCls);
}

void CSharpSourceGenerator::GenerateDelegates(CodeBlock &codeBlock,
                                              const CSharpClass &csCls) const {
  for (auto &delegate: csCls.mDelegates) {
    GenerateDelegate(codeBlock, csCls, delegate);
  }
  if (!csCls.mDelegates.empty())
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateDelegate(CodeBlock &codeBlock, const CSharpClass &csCls,
                                             const CSharpMethod &method) const {
  (void)csCls;
  codeBlock.Add("{} delegate {} {}({});", method.mVisibility, method.mReturnType.ToString(),
                method.mName, GenerateArgumentsInSignature(method));
}

void CSharpSourceGenerator::GenerateMethods(CodeBlock &codeBlock, const CSharpClass &csCls) const {
  for (auto &method: csCls.mMethods) {
    GenerateMethod(codeBlock, csCls, method);
  }
  if (!csCls.mMethods.empty())
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateMethod(CodeBlock &codeBlock, const CSharpClass &csCls,
                                           const CSharpMethod &method) const {
  (void)csCls;
  std::string virtualityStr;
  bool hasBody = true;
  if (method.mVirtuality == Virtuality::Virtual) {
    virtualityStr = " virtual";
  } else if (method.mVirtuality == Virtuality::PureVirtual) {
    virtualityStr = " abstract";
    hasBody = false;
  }
  codeBlock.Add("{}{}{} {} {}({}){}", method.mVisibility,
                method.mStaticness == Staticness::Static ? " static" : "", virtualityStr,
                method.mReturnType.ToString(), method.mName, GenerateArgumentsInSignature(method),
                hasBody ? "" : ";");
  if (!hasBody)
    return;
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add(method.mBody);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

std::string CSharpSourceGenerator::GenerateArgumentsInSignature(const CSharpMethod &method) const {
  std::stringstream ss;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    if (!arg.mAttributes.empty()) {
      ss << "[";
      std::string delim;
      for(auto& attrib: arg.mAttributes) {
        ss << delim << attrib;
        delim = ", ";
      }
      ss << "] ";
    }
    ss << arg.mType.ToString() << " " << arg.mName;
    if (arg.mDefaultValue.has_value()) {
      ss << " = " << *arg.mDefaultValue;
    }
  }
  return ss.str();
}

} // namespace holgen
