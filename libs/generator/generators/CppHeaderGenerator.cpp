#include "CppHeaderGenerator.h"

#include "core/St.h"

namespace holgen {

namespace {
bool ShouldGenerateMethod(const MethodBase &method, Visibility visibility, bool isInsideClass) {
  if (method.mVisibility != visibility)
    return false;
  if (method.mDefaultDelete != DefaultDelete::Neither)
    return isInsideClass;
  if (isInsideClass && method.mIsTemplateSpecialization)
    return false;
  if (!isInsideClass && !method.mIsTemplateSpecialization)
    return false;
  return true;
}
} // namespace

void CppHeaderGenerator::Run(std::vector<GeneratedContent> &contents) const {
  for (auto &cls: mTranslatedProject.mClasses) {
    Generate(contents.emplace_back(), cls);
  }
}

void CppHeaderGenerator::Generate(GeneratedContent &out, const Class &cls) const {
  auto headers = PrepareIncludes(cls, true);
  out.mType = FileType::CppHeader;
  out.mName = std::format("gen/{}.h", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", St::GenMessage);
  codeBlock.Add("#pragma once");
  codeBlock.Line();
  codeBlock.Add("#include \"../holgen.h\"");
  headers.Write(codeBlock);
  codeBlock.Add(cls.mHeaderCustomSection);
  GenerateClassDefinition(cls, codeBlock);
  for (auto &specialization: cls.mSpecializations) {
    GenerateClassDefinition(specialization, codeBlock);
  }
  GenerateCFunctionsForHeader(codeBlock, cls);
  GenerateMacros(codeBlock, cls);
  out.mBody = std::move(codeBlock);
}

void CppHeaderGenerator::GenerateClassDefinition(const Class &cls, CodeBlock &codeBlock) const {
  if (!cls.mNamespace.empty())
    codeBlock.Add("namespace {} {{", cls.mNamespace);

  // TODO: struct-specific namespaces defined via annotations
  AddCppComments(codeBlock, cls.mComments);
  if (cls.mStruct) {
    codeBlock.Add("// Defined in {}", cls.mStruct->mDefinitionSource.mSource);
  } else if (cls.mEnum) {
    codeBlock.Add("// Defined in {}", cls.mEnum->mDefinitionSource.mSource);
  }
  if (!cls.mTemplateParameters.empty() || !cls.mTemplateSpecializations.empty())
    codeBlock.AddLine(StringifyTemplateParameters(cls.mTemplateParameters));
  codeBlock.Add("{} {{", GenerateClassDeclaration(cls));
  GenerateClassFriends(cls, codeBlock);
  GenerateForVisibility(codeBlock, cls, Visibility::Public);
  GenerateForVisibility(codeBlock, cls, Visibility::Protected);
  GenerateForVisibility(codeBlock, cls, Visibility::Private);
  codeBlock.Add("}};"); // class
  GenerateMethodsForHeader(codeBlock, cls, Visibility::Public, false);
  GenerateMethodsForHeader(codeBlock, cls, Visibility::Protected, false);
  GenerateMethodsForHeader(codeBlock, cls, Visibility::Private, false);
  if (!cls.mNamespace.empty())
    codeBlock.Add("}}");
}

void CppHeaderGenerator::GenerateClassFriends(const Class &cls, CodeBlock &codeBlock) const {
  for (auto &friendClassType: cls.mFriendClasses) {
    auto friendClass = mTranslatedProject.GetClass(friendClassType.mName);
    if (friendClass) {
      codeBlock.Add("friend {};", GenerateClassDeclaration(*friendClass));
    } else {
      codeBlock.Add("friend class {};", friendClassType.mName);
    }
  }
  if (!cls.mFriendClasses.empty())
    codeBlock.Add(""); // class
}

std::string CppHeaderGenerator::GenerateClassDeclaration(const Class &cls) const {
  std::stringstream line;
  if (cls.mClassType == ClassType::Struct)
    line << "struct ";
  else
    line << "class ";
  line << cls.mName;
  if (!cls.mTemplateSpecializations.empty()) {
    line << "<";
    bool isFirst = true;
    for (const auto &templateSpecialization: cls.mTemplateSpecializations) {
      if (isFirst) {
        isFirst = false;
      } else {
        line << ", ";
      }
      line << templateSpecialization;
    }
    line << ">";
  }
  if (!cls.mBaseClasses.empty()) {
    line << " :";
    bool isFirst = true;
    for (const auto &baseClass: cls.mBaseClasses) {
      if (isFirst) {
        isFirst = false;
      } else {
        line << ",";
      }
      line << std::format(" {} {}", baseClass.mVisibility, baseClass.mType.ToString(true));
    }
  }
  return line.str();
}

void CppHeaderGenerator::GenerateForVisibility(CodeBlock &codeBlock, const Class &cls,
                                               Visibility visibility) const {
  CodeBlock codeBlockForVisibility;
  if (visibility == Visibility::Public)
    GenerateUsingsForHeader(codeBlockForVisibility, cls);
  GenerateNestedEnumsForHeader(codeBlockForVisibility, cls, visibility);
  GenerateConstructorsForHeader(codeBlockForVisibility, cls, visibility, true);
  auto dtorCodeBlock = GenerateDestructor(cls, visibility, true);
  if (!dtorCodeBlock.mContents.empty()) {
    codeBlockForVisibility.Add(std::move(dtorCodeBlock));
  }
  GenerateMethodsForHeader(codeBlockForVisibility, cls, visibility, true);
  GenerateFieldDeclarations(codeBlockForVisibility, cls, visibility);
  if (!codeBlockForVisibility.mContents.empty()) {
    codeBlock.Add("{}:", visibility);
    codeBlock.Indent(1);
    codeBlock.Add(std::move(codeBlockForVisibility));
    codeBlock.Indent(-1);
  }
}

void CppHeaderGenerator::GenerateUsingsForHeader(CodeBlock &codeBlock, const Class &cls) const {
  for (auto &usingStatement: cls.mUsings) {
    codeBlock.Add("using {}={};", usingStatement.mTargetType,
                  usingStatement.mSourceType.ToString(true));
  }
}

void CppHeaderGenerator::GenerateNestedEnumsForHeader(CodeBlock &codeBlock, const Class &cls,
                                                      Visibility visibility) const {
  for (auto &nestedEnum: cls.mNestedEnums) {
    if (nestedEnum.mVisibility != visibility)
      continue;
    AddCppComments(codeBlock, nestedEnum.mComments);
    if (nestedEnum.mUnderlyingType.empty())
      codeBlock.Add("enum {} {{", nestedEnum.mName);
    else
      codeBlock.Add("enum {} : {} {{", nestedEnum.mName, nestedEnum.mUnderlyingType);
    codeBlock.Indent(1);
    for (auto &entry: nestedEnum.mEntries) {
      AddCppComments(codeBlock, entry.mComments);
      if (entry.mValue.empty())
        codeBlock.Add("{},", entry.mName);
      else
        codeBlock.Add("{} = {},", entry.mName, entry.mValue);
    }
    codeBlock.Indent(-1);
    codeBlock.Add("}};");
  }
}

void CppHeaderGenerator::GenerateConstructorsForHeader(CodeBlock &codeBlock, const Class &cls,
                                                       Visibility visibility,
                                                       bool isInsideClass) const {
  for (auto &ctor: cls.mConstructors) {
    if (!ShouldGenerateMethod(ctor, visibility, isInsideClass))
      continue;

    if (!ctor.mTemplateParameters.empty())
      codeBlock.AddLine(StringifyTemplateParameters(ctor.mTemplateParameters));
    if (ctor.mIsTemplateSpecialization)
      codeBlock.Line() << "template <>";

    bool willDefine = !ctor.mTemplateParameters.empty() || !cls.mTemplateParameters.empty();

    {
      auto line = codeBlock.Line();
      line << GenerateFunctionSignature(cls, ctor, true, isInsideClass);
      if (ctor.mDefaultDelete == DefaultDelete::Default) {
        line << " = default;";
        continue;
      } else if (ctor.mDefaultDelete == DefaultDelete::Delete) {
        line << " = delete;";
        continue;
      } else if (!willDefine) {
        line << ";";
        continue;
      } else {
        line << " {";
      }
    }
    codeBlock.Indent(1);
    codeBlock.Add(ctor.mBody);
    codeBlock.Indent(-1);
    codeBlock.Line() << "}";
  }
}

void CppHeaderGenerator::GenerateMethodsForHeader(CodeBlock &codeBlock, const Class &cls,
                                                  Visibility visibility, bool isInsideClass) const {
  for (auto &method: cls.mMethods) {
    if (!ShouldGenerateMethod(method, visibility, isInsideClass))
      continue;
    AddCppComments(codeBlock, method.mComments);
    if (!method.mTemplateParameters.empty())
      codeBlock.AddLine(StringifyTemplateParameters(method.mTemplateParameters));
    if (method.mIsTemplateSpecialization)
      codeBlock.Line() << "template <>";

    auto signature = GenerateFunctionSignature(cls, method, true, isInsideClass);
    if (method.mFunctionPointer) {
      if (!method.IsStatic(cls)) {
        codeBlock.Line() << signature << " = nullptr;";
      } else {
        codeBlock.Line() << "inline " << signature << " = nullptr;";
      }
    } else if (method.mVirtuality == Virtuality::PureVirtual) {
      codeBlock.Line() << signature << " = 0;";
    } else if (!CanBeDefinedInHeader(cls, method) || method.mUserDefined) {
      codeBlock.Line() << signature << ";";
    } else if (method.mDefaultDelete == DefaultDelete::Default) {
      codeBlock.Line() << signature << " = default;";
    } else if (method.mDefaultDelete == DefaultDelete::Delete) {
      codeBlock.Line() << signature << " = delete;";
    } else {
      codeBlock.Line() << signature << " {";
      codeBlock.Indent(1);
      codeBlock.Add(method.mBody);
      codeBlock.Indent(-1);
      codeBlock.Line() << "}";
    }
  }
}

void CppHeaderGenerator::GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls,
                                                   Visibility visibility) const {
  for (auto &field: cls.mFields) {
    if (field.mVisibility != visibility)
      continue;
    {
      bool canDefineInline = true;
      if (field.mType.mName == cls.mName && field.mStaticness == Staticness::Static)
        canDefineInline = false;
      AddCppComments(codeBlock, field.mComments);
      auto line = codeBlock.Line();
      if (field.mStaticness == Staticness::Static) {
        if (canDefineInline && field.mType.mConstexprness == Constexprness::NotConstexpr)
          line << "inline static ";
        else
          line << "static ";
      }
      line << field.mType.ToString(false) << field.mName;
      if (canDefineInline)
        line << StringifyFieldDefinition(field);
      line << ";";
    }
  }
}

void CppHeaderGenerator::GenerateCFunctionsForHeader(CodeBlock &codeBlock, const Class &cls) const {
  if (cls.mCFunctions.empty())
    return;
  codeBlock.Add("extern \"C\" {{");
  codeBlock.Indent(1);
  for (auto &func: cls.mCFunctions) {
    codeBlock.Add("{};", GenerateFunctionSignature(func, true));
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void CppHeaderGenerator::GenerateMacros(CodeBlock &codeBlock, const Class &cls) const {
  for (auto &macro: cls.mHeaderMacros) {
    if (macro.mArguments.empty()) {
      codeBlock.Add("#define {} \\", macro.mName);
    } else {
      std::stringstream ss;
      bool isFirst = true;
      for (auto &argument: macro.mArguments) {
        if (isFirst) {
          isFirst = false;
        } else {
          ss << ", ";
        }
        ss << argument;
      }
      codeBlock.Add("#define {}({}) \\", macro.mName, ss.str());
    }
    codeBlock.Indent(2);
    auto curLineCount = codeBlock.mLines.size();
    codeBlock.Add(macro.mBody);
    for (size_t i = curLineCount; i < codeBlock.mLines.size() - 1; ++i) {
      codeBlock.mLines[i] += " \\";
    }
    codeBlock.Indent(-2);
  }
}

} // namespace holgen
