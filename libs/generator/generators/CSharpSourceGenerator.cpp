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
  codeBlock.Add("// {}", St::GenMessage);
  codeBlock.Add("");
  GenerateUsingDirectives(codeBlock, csCls);
  GenerateClassNamespace(codeBlock);
  GenerateClass(codeBlock, csCls);
  out.mBody = std::move(codeBlock);
}

void CSharpSourceGenerator::GenerateClass(CodeBlock &codeBlock, const CSharpClass &csCls) const {
  GenerateAttributes(codeBlock, csCls.mAttributes);
  codeBlock.Add("{} {}{}{} {}", csCls.mVisibility,
                csCls.mStaticness == Staticness::Static ? "static " : "",
                csCls.mIsAbstract ? "abstract " : "", csCls.mType, csCls.mName);
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  GenerateClassBody(codeBlock, csCls);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
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
  GenerateClassBody(codeBlock, csCls, CSharpVisibility::Public);
  GenerateClassBody(codeBlock, csCls, CSharpVisibility::Protected);
  GenerateClassBody(codeBlock, csCls, CSharpVisibility::Private);
  GenerateClassBody(codeBlock, csCls, CSharpVisibility::Internal);
}

void CSharpSourceGenerator::GenerateClassBody(CodeBlock &codeBlock, const CSharpClass &csCls,
                                              CSharpVisibility visibility) const {
  GenerateConstructors(codeBlock, csCls, visibility);
  GenerateFields(codeBlock, csCls, visibility);
  GenerateInnerClasses(codeBlock, csCls, visibility);
  GenerateMethods(codeBlock, csCls, visibility);
  GenerateDelegates(codeBlock, csCls, visibility);
}

void CSharpSourceGenerator::GenerateConstructors(CodeBlock &codeBlock, const CSharpClass &csCls,
                                                 CSharpVisibility visibility) const {
  bool added = false;
  for (auto &ctor: csCls.mConstructors) {
    if (ctor.mVisibility != visibility)
      continue;
    GenerateConstructor(codeBlock, csCls, ctor);
    added = true;
  }
  if (added)
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateConstructor(CodeBlock &codeBlock, const CSharpClass &csCls,
                                                const CSharpConstructor &ctor) const {
  codeBlock.Add("{} {}({})", ctor.mVisibility, csCls.mName, GenerateArgumentsInSignature(ctor));
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add(ctor.mBody);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void CSharpSourceGenerator::GenerateDelegates(CodeBlock &codeBlock, const CSharpClass &csCls,
                                              CSharpVisibility visibility) const {
  bool added = false;
  for (auto &delegate: csCls.mDelegates) {
    if (delegate.mVisibility != visibility)
      continue;
    GenerateDelegate(codeBlock, csCls, delegate);
    added = true;
  }
  if (added)
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateDelegate(CodeBlock &codeBlock, const CSharpClass &csCls,
                                             const CSharpMethod &method) const {
  (void)csCls;
  GenerateAttributes(codeBlock, method.mAttributes);
  codeBlock.Add("{} delegate {} {}({});", method.mVisibility, method.mReturnType.ToString(),
                method.mName, GenerateArgumentsInSignature(method));
}

void CSharpSourceGenerator::GenerateMethods(CodeBlock &codeBlock, const CSharpClass &csCls,
                                            CSharpVisibility visibility) const {
  bool added = false;
  for (auto &method: csCls.mMethods) {
    if (method.mVisibility != visibility)
      continue;
    GenerateMethod(codeBlock, csCls, method);
    added = true;
  }
  if (added)
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateMethod(CodeBlock &codeBlock, const CSharpClass &csCls,
                                           const CSharpMethod &method) const {
  (void)csCls;
  std::string virtualityStr;
  bool hasBody = true;
  GenerateAttributes(codeBlock, method.mAttributes);
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

void CSharpSourceGenerator::GenerateFields(CodeBlock &codeBlock, const CSharpClass &csCls,
                                           CSharpVisibility visibility) const {
  bool added = false;
  for (auto &field: csCls.mFields) {
    if (field.mVisibility != visibility)
      continue;
    GenerateField(codeBlock, csCls, field);
    added = true;
  }
  if (added)
    codeBlock.Add("");
}

void CSharpSourceGenerator::GenerateField(CodeBlock &codeBlock, const CSharpClass &csCls,
                                          const CSharpClassField &field) const {
  (void)csCls;
  std::string defaultValPart;
  if (field.mDefaultValue.has_value()) {
    defaultValPart = std::format(" = {}", *field.mDefaultValue);
  }
  bool simpleDefinition = (!field.mGetter.has_value() || field.mGetter->mBody.IsEmpty()) &&
      (!field.mSetter.has_value() || field.mSetter->mBody.IsEmpty());
  auto initialPart = std::format("{}{} {} {}", field.mVisibility,
                                 field.mStaticness == Staticness::Static ? " static" : "",
                                 field.mType.ToString(), field.mName);
  if (simpleDefinition) {
    if (field.mDirectTo.has_value()) {
      codeBlock.Add("{}{} => {};", initialPart, defaultValPart, *field.mDirectTo);
    } else if (!field.mGetter.has_value() && !field.mSetter.has_value()) {
      codeBlock.Add("{}{};", initialPart, defaultValPart);
    } else {
      std::stringstream innerPart;
      innerPart << " {";
      if (field.mGetter.has_value()) {
        if (field.mGetter->mVisibility != field.mVisibility) {
          innerPart << std::format(" {}", field.mGetter->mVisibility);
        }
        innerPart << " get;";
      }
      if (field.mSetter.has_value()) {
        if (field.mSetter->mVisibility != field.mVisibility) {
          innerPart << std::format(" {}", field.mSetter->mVisibility);
        }
        innerPart << " set;";
      }
      innerPart << " }";
      codeBlock.Add("{}{}{}", initialPart, innerPart.str(), defaultValPart);
    }
  } else {
    codeBlock.Add("{}", initialPart);
    codeBlock.Add("{{");
    codeBlock.Indent(1);
    if (field.mGetter.has_value()) {
      GenerateFieldAccessor(codeBlock, *field.mGetter, field, "get");
    }
    if (field.mSetter.has_value()) {
      GenerateFieldAccessor(codeBlock, *field.mSetter, field, "set");
    }
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
  }
}

void CSharpSourceGenerator::GenerateInnerClasses(CodeBlock &codeBlock, const CSharpClass &csCls,
                                                 CSharpVisibility visibility) const {
  if (visibility != CSharpVisibility::Public)
    return;
  for (auto &innerClass: csCls.mInnerClasses) {
    GenerateClass(codeBlock, innerClass);
    codeBlock.Add("");
  }
}

void CSharpSourceGenerator::GenerateFieldAccessor(CodeBlock &codeBlock,
                                                  const CSharpMethodBase &method,
                                                  const CSharpClassField &field,
                                                  const std::string &name) const {
  std::string visibility;
  if (method.mVisibility != field.mVisibility) {
    visibility = std::format("{} ", method.mVisibility);
  }
  if (method.mBody.mContents.size() == 1 && method.mBody.mContents[0] == CodeUnitType::Code) {
    codeBlock.Add("{}{} => {}", visibility, name, method.mBody.mLines[0]);
    return;
  }
  codeBlock.Add("{}{}", visibility, name);
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add(method.mBody);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

std::string
    CSharpSourceGenerator::GenerateArgumentsInSignature(const CSharpMethodBase &method) const {
  std::stringstream ss;
  bool isFirst = true;
  for (auto &arg: method.mArguments) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    auto attributesString = GenerateAttributes(arg.mAttributes);
    if (!attributesString.empty()) {
      ss << attributesString << " ";
    }
    ss << arg.mType.ToString() << " " << arg.mName;
    if (arg.mDefaultValue.has_value()) {
      ss << " = " << *arg.mDefaultValue;
    }
  }
  return ss.str();
}

std::string
    CSharpSourceGenerator::GenerateAttributes(const std::list<std::string> &attributes) const {
  std::stringstream ss;
  if (!attributes.empty()) {
    ss << "[";
    std::string delim;
    for (auto &attrib: attributes) {
      ss << delim << attrib;
      delim = ", ";
    }
    ss << "]";
  }
  return ss.str();
}

void CSharpSourceGenerator::GenerateAttributes(CodeBlock &codeBlock,
                                               const std::list<std::string> &attributes) const {
  auto res = GenerateAttributes(attributes);
  if (!res.empty()) {
    codeBlock.Add("{}", res);
  }
}

} // namespace holgen
