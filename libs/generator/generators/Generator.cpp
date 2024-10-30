#include "Generator.h"

namespace holgen {

std::string Generator::GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
                                                 bool isInHeader, bool isInsideClass) const {
  std::stringstream ss;
  if (method.mConstexprness == Constexprness::Constexpr)
    ss << "constexpr ";
  if (method.mStaticness == Staticness::Static && isInsideClass && isInHeader)
    ss << "static ";
  if (isInHeader && isInsideClass && method.mVirtuality != Virtuality::NotVirtual) {
    ss << "virtual ";
  }
  // TODO: this doesn't work if type is const
  if (!isInHeader && cls.GetUsing(method.mReturnType.mName))
    ss << cls.mName << "::";
  ss << method.mReturnType.ToString(false);
  if (!isInHeader || !isInsideClass)
    ss << cls.mName << "::";
  if (method.mFunctionPointer)
    ss << "(*";
  ss << method.mName;
  if (method.mFunctionPointer)
    ss << ")";
  ss << "(";
  size_t idx = 0;
  for (auto &arg: method.mArguments) {
    if (idx != 0)
      ss << ", ";
    ss << arg.mType.ToString(false) << arg.mName;
    if (isInHeader && arg.mDefaultValue.has_value())
      ss << " = " << *arg.mDefaultValue;
    ++idx;
  }
  ss << ")";
  if (method.mConstness == Constness::Const)
    ss << " const";
  if (method.mNoexceptness == Noexceptness::Noexcept)
    ss << " noexcept";
  return ss.str();
}

std::string Generator::GenerateFunctionSignature(const Class &cls, const ClassConstructor &ctor,
                                                 bool isInHeader, bool isInsideClass) const {
  std::stringstream ss;
  if (isInHeader && isInsideClass && ctor.mExplicitness == Explicitness::Explicit)
    ss << "explicit ";
  if (!isInsideClass)
    ss << cls.mName << "::";
  ss << cls.mName << "(";
  bool isFirst = true;
  for (auto &arg: ctor.mArguments) {
    if (isFirst)
      isFirst = false;
    else
      ss << ", ";
    // TODO: this doesn't work if type is const
    if (cls.GetUsing(arg.mType.mName) && !isInsideClass)
      ss << cls.mName << "::";
    ss << arg.mType.ToString(false) << arg.mName;
    if (isInsideClass && arg.mDefaultValue.has_value()) {
      ss << " = " << *arg.mDefaultValue;
    }
  }
  ss << ")";
  if (ctor.mNoexceptness == Noexceptness::Noexcept)
    ss << " noexcept";
  return ss.str();
}

std::string Generator::GenerateFunctionSignature(const CFunction &func, bool isInHeader) const {
  std::stringstream ss;
  if (isInHeader) {
    ss << "HOLGEN_EXPORT ";
  }
  ss << func.mReturnType.ToFullyQualifiedString(mTranslatedProject);

  ss << func.mName << "(";
  size_t idx = 0;
  for (auto &arg: func.mArguments) {
    if (idx != 0)
      ss << ", ";
    ss << arg.mType.ToFullyQualifiedString(mTranslatedProject) << arg.mName;
    ++idx;
  }
  ss << ")";
  return ss.str();
}

HeaderContainer Generator::PrepareIncludes(const Class &cls, bool isHeader) const {
  HeaderContainer headers;
  if (isHeader)
    headers = cls.mHeaderIncludes;
  else
    headers = cls.mSourceIncludes;

  if (isHeader) {
    for (auto &baseClass: cls.mBaseClasses) {
      headers.IncludeBaseClass(mTranslatedProject, cls, baseClass);
    }
  }

  for (const auto &field: cls.mFields) {
    headers.IncludeClassField(mTranslatedProject, cls, field, isHeader);
  }
  for (const auto &method: cls.mMethods) {
    headers.IncludeClassMethod(mTranslatedProject, cls, method, isHeader);
  }
  for (const auto &usingStatement: cls.mUsings) {
    headers.IncludeUsing(mTranslatedProject, usingStatement, isHeader);
  }
  return headers;
}

void Generator::AddCppComments(CodeBlock &codeBlock, const std::list<std::string> &comments) const {
  if (comments.empty())
    return;
  if (!comments.empty()) {
    if (comments.size() == 1) {
      codeBlock.Add("// {}", comments.front());
    } else {
      codeBlock.Add("/*");
      for (auto line: comments) {
        while (true) {
          auto idx = line.find("*/");
          if (idx == std::string::npos)
            break;
          line.replace(idx, 2, "* /");
        }
        codeBlock.Add(" * {}", line);
      }
      codeBlock.Add(" */");
    }
  }
}

bool Generator::CanBeDefinedInHeader(const Class &cls, const MethodBase &method) const {
  return !method.mTemplateParameters.empty() || !cls.mTemplateParameters.empty() ||
      !cls.mTemplateSpecializations.empty() || method.mDefaultDelete != DefaultDelete::Neither;
}

bool Generator::CanBeDefinedInHeader(const Class &cls, const ClassMethod &method) const {
  return CanBeDefinedInHeader(cls, (const MethodBase &)method) ||
      method.mConstexprness == Constexprness::Constexpr;
}

std::string Generator::StringifyFieldDefinition(const ClassField &field) const {
  std::stringstream ss;
  if (!field.mDefaultConstructorArguments.empty()) {
    ss << "(";
    bool isFirst = true;
    for (auto &ctorArg: field.mDefaultConstructorArguments) {
      if (isFirst)
        isFirst = false;
      else
        ss << ", ";
      ss << ctorArg;
    }
    ss << ")";
  } else if (field.mDefaultValue.has_value()) {
    ss << " = " << *field.mDefaultValue;
  }
  return ss.str();
}

CodeBlock Generator::GenerateDestructor(const Class &cls, Visibility visibility,
                                        bool isHeader) const {
  CodeBlock codeBlock;
  if (cls.mDestructor.IsEmpty() || (isHeader && cls.mDestructor.mVisibility != visibility))
    return {};
  bool definedInHeader = CanBeDefinedInHeader(cls, cls.mDestructor);
  std::string virtualPrefix = cls.HasVirtualMethods() ? "virtual " : "";
  if (isHeader) {
    if (!definedInHeader) {
      codeBlock.Add("{}~{}();", virtualPrefix, cls.mName);
    } else if (cls.mDestructor.mDefaultDelete == DefaultDelete::Default) {
      codeBlock.Add("{}~{}() = default;", virtualPrefix, cls.mName);
    } else if (cls.mDestructor.mDefaultDelete == DefaultDelete::Delete) {
      codeBlock.Add("{}~{}() = delete;", virtualPrefix, cls.mName);
    } else {
      if (isHeader) {
        codeBlock.Add("{}~{}() {{", virtualPrefix, cls.mName);
      } else {
        codeBlock.Add("~{}() {{", cls.mName);
      }
      codeBlock.Indent(1);
      codeBlock.Add(cls.mDestructor.mBody);
      codeBlock.Indent(-1);
      codeBlock.Add("}}");
    }
  } else if (!definedInHeader) {
    codeBlock.Add("{0}::~{0}() {{", cls.mName);
    codeBlock.Indent(1);
    codeBlock.Add(cls.mDestructor.mBody);
    codeBlock.Indent(-1);
    codeBlock.Add("}}");
  }
  return codeBlock;
}

} // namespace holgen
