#include "CodeGenerator.h"

#include <holgen.h>
#include <sstream>
#include "HeaderContainer.h"

namespace holgen {
namespace {
const char *GenMessage = "This file is generated by holgen. Do not modify manually.";
const char *PartialGenMessage =
    "This file is partially generated by holgen. Only modify the custom sections.";
} // namespace

namespace {
bool CanBeDefinedInHeader(const Class &cls, const ClassMethodBase &method) {
  return !method.mTemplateParameters.empty() || !cls.mTemplateParameters.empty() ||
      !cls.mTemplateSpecializations.empty() || method.mDefaultDelete != DefaultDelete::Neither;
}

bool CanBeDefinedInHeader(const Class &cls, const ClassMethod &method) {
  return CanBeDefinedInHeader(cls, (const ClassMethodBase &)method) ||
      method.mConstexprness == Constexprness::Constexpr;
}

template <typename C>
std::string StringifyTemplateParameters(const C &templateParameters) {
  std::stringstream ss;
  ss << "template <";
  bool isFirst = true;
  for (const auto &templateParameter: templateParameters) {
    if (isFirst) {
      isFirst = false;
    } else {
      ss << ", ";
    }
    ss << templateParameter.mType << " " << templateParameter.mName;
  }
  ss << ">";
  return ss.str();
}

void AddComments(CodeBlock &codeBlock, const std::list<std::string> &comments) {
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

std::string StringifyFieldDefinition(const ClassField &field) {
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

bool HasUserDefinedMethods(const Class &cls) {
  for (const auto &method: cls.mMethods) {
    if (method.mUserDefined) {
      return true;
    }
  }
  for (const auto &ctor: cls.mConstructors) {
    if (ctor.mUserDefined) {
      return true;
    }
  }
  return false;
}
} // namespace

std::vector<GeneratedContent> CodeGenerator::Generate(const TranslatedProject &translatedProject) {
  mTranslatedProject = &translatedProject;
  std::vector<GeneratedContent> contents;
  for (auto &cls: translatedProject.mClasses) {
    auto headerHeaders = PrepareIncludes(cls, true);
    auto sourceHeaders = PrepareIncludes(cls, false);
    sourceHeaders.Subtract(headerHeaders);
    GenerateClassHeader(contents.emplace_back(), cls, headerHeaders);
    GenerateClassSource(contents.emplace_back(), cls, sourceHeaders);
    if (HasUserDefinedMethods(cls)) {
      GenerateClassModifiableSource(contents.emplace_back(), cls);
    }
  }
  GenerateHolgenHeader(contents.emplace_back());
  GenerateCMakeLists(contents.emplace_back(), translatedProject);

  if (mGeneratorSettings.IsFeatureEnabled(GeneratorFeatureFlag::SwigMask)) {
    GenerateSwigInterface(contents.emplace_back(), translatedProject);
  }

  mTranslatedProject = nullptr;

  return contents;
}

void CodeGenerator::GenerateClassHeader(GeneratedContent &header, const Class &cls,
                                        const HeaderContainer &headers) const {
  header.mType = FileType::CppHeader;
  header.mName = std::format("gen/{}.h", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", GenMessage);
  codeBlock.Add("#pragma once");
  codeBlock.Line();
  codeBlock.Add("#include \"../holgen.h\"");
  headers.Write(codeBlock);
  GenerateClassDefinition(cls, codeBlock);
  for (auto &specialization: cls.mSpecializations) {
    GenerateClassDefinition(specialization, codeBlock);
  }
  header.mBody = std::move(codeBlock);
}

void CodeGenerator::GenerateClassDefinition(const Class &cls, CodeBlock &codeBlock) const {
  if (!cls.mNamespace.empty())
    codeBlock.Add("namespace {} {{", cls.mNamespace);
  GenerateClassDeclarationsForHeader(codeBlock, cls);
  // TODO: struct-specific namespaces defined via annotations
  AddComments(codeBlock, cls.mComments);
  if (cls.mStruct) {
    codeBlock.Add("// Defined in {}", cls.mStruct->mDefinitionSource.mSource);
  } else if (cls.mEnum) {
    codeBlock.Add("// Defined in {}", cls.mEnum->mDefinitionSource.mSource);
  }
  if (!cls.mTemplateParameters.empty() || !cls.mTemplateSpecializations.empty())
    codeBlock.AddLine(StringifyTemplateParameters(cls.mTemplateParameters));
  codeBlock.Add("{} {{", GenerateClassDeclaration(cls));
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

void CodeGenerator::GenerateSwigInterface(GeneratedContent &swig,
                                          const TranslatedProject &translatedProject) const {
  swig.mType = FileType::SwigInterface;
  swig.mName = std::format("swig.i", mGeneratorSettings.mProjectName);
  CodeBlock includes;
  CodeBlock body;
  for (auto &cls: translatedProject.mClasses) {
    if (cls.mStruct) {
      GenerateSwigInterfaceIncludes(cls, includes);
      GenerateSwigInterfaceForStruct(cls, body);
    } else if (cls.mEnum) {
      GenerateSwigInterfaceIncludes(cls, includes);
      GenerateSwigInterfaceForEnum(cls, body);
    }
  }
  swig.mBody.Add("%module {}", mGeneratorSettings.mProjectName);
  swig.mBody.Add("%{{", mGeneratorSettings.mProjectName);
  swig.mBody.Add(std::move(includes));
  swig.mBody.Add("%}}", mGeneratorSettings.mProjectName);
  swig.mBody.Add(std::move(body));
}

void CodeGenerator::GenerateSwigInterfaceIncludes(const Class &cls, CodeBlock &codeBlock) const {
  codeBlock.Add("#include \"../gen/{}.h\"", cls.mName);
}

void CodeGenerator::GenerateSwigInterfaceForStruct(const Class &cls, CodeBlock &codeBlock) const {
  codeBlock.Add("class {} {{", cls.mName);
  codeBlock.Add("public:");
  codeBlock.Indent(1);
  for (auto &ctor: cls.mConstructors) {
    if (ctor.mVisibility == Visibility::Public)
      codeBlock.Add("{};", GenerateFunctionSignature(cls, ctor, true, true));
  }
  // dtor
  codeBlock.Add("~{}();", cls.mName);
  for (auto &method: cls.mMethods) {
    if (method.mVisibility == Visibility::Public)
      codeBlock.Add("{};", GenerateFunctionSignature(cls, method, true, true));
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}};");
}

void CodeGenerator::GenerateSwigInterfaceForEnum(const Class &cls, CodeBlock &codeBlock) const {
  codeBlock.Add("class {} {{", cls.mName);
  codeBlock.Add("public:");
  codeBlock.Indent(1);
  {
    auto line = codeBlock.Line();
    line << "enum {";
    auto &enumDefinition = *cls.mEnum;
    bool isFirst = true;
    for (auto &entry: enumDefinition.mEntries) {
      if (isFirst) {
        isFirst = false;
      } else {
        line << ", ";
      }
      line << entry.mName << "=" << entry.mValue;
    }
    line << "}";
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}};");
}

std::string CodeGenerator::GenerateClassDeclaration(const Class &cls) const {
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
      line << " public " << baseClass;
    }
  }
  return line.str();
}

void CodeGenerator::GenerateForVisibility(CodeBlock &codeBlock, const Class &cls,
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
    switch (visibility) {
    case Visibility::Public:
      codeBlock.Line() << "public:";
      break;
    case Visibility::Protected:
      codeBlock.Line() << "protected:";
      break;
    case Visibility::Private:
      codeBlock.Line() << "private:";
      break;
    }
    codeBlock.Indent(1);
    codeBlock.Add(std::move(codeBlockForVisibility));
    codeBlock.Indent(-1);
  }
}

void CodeGenerator::GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls,
                                              Visibility visibility) const {
  for (auto &field: cls.mFields) {
    if (field.mVisibility != visibility)
      continue;
    {
      bool canDefineInline = true;
      if (field.mType.mName == cls.mName && field.mStaticness == Staticness::Static)
        canDefineInline = false;
      AddComments(codeBlock, field.mComments);
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

namespace {
bool ShouldGenerateMethod(const ClassMethodBase &method, Visibility visibility,
                          bool isInsideClass) {
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

void CodeGenerator::GenerateConstructorsForHeader(CodeBlock &codeBlock, const Class &cls,
                                                  Visibility visibility, bool isInsideClass) const {
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

void CodeGenerator::GenerateMethodsForHeader(CodeBlock &codeBlock, const Class &cls,
                                             Visibility visibility, bool isInsideClass) const {
  for (auto &method: cls.mMethods) {
    if (!ShouldGenerateMethod(method, visibility, isInsideClass))
      continue;

    AddComments(codeBlock, method.mComments);

    if (!method.mTemplateParameters.empty())
      codeBlock.AddLine(StringifyTemplateParameters(method.mTemplateParameters));
    if (method.mIsTemplateSpecialization)
      codeBlock.Line() << "template <>";

    auto signature = GenerateFunctionSignature(cls, method, true, isInsideClass);
    if (!CanBeDefinedInHeader(cls, method) || method.mUserDefined) {
      codeBlock.Line() << signature << ";";
      continue;
    }
    if (method.mDefaultDelete == DefaultDelete::Default) {
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

void CodeGenerator::GenerateClassSource(GeneratedContent &source, const Class &cls,
                                        const HeaderContainer &headers) const {
  source.mType = FileType::CppSource;
  source.mName = std::format("gen/{}.cpp", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", GenMessage);
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

  source.mBody = std::move(codeBlock);
}

CodeBlock CodeGenerator::GenerateFieldsForSource(const Class &cls) const {
  CodeBlock codeBlock;
  for (auto &field: cls.mFields) {
    if (field.mStaticness == Staticness::Static && field.mType.mName == cls.mName) {
      codeBlock.Add("{}{}::{}{};", field.mType.ToString(false), cls.mName, field.mName,
                    StringifyFieldDefinition(field));
    }
  }
  return codeBlock;
}

CodeBlock CodeGenerator::GenerateMethodsForSource(const Class &cls) const {
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

void CodeGenerator::GenerateCMakeLists(GeneratedContent &cmake,
                                       const TranslatedProject &translatedProject) const {
  cmake.mType = FileType::CMakeFile;
  cmake.mName = "CMakeLists.txt";
  CodeBlock codeBlock;
  codeBlock.Add("# {}", PartialGenMessage);
  codeBlock.Add("set(gen_sources");
  codeBlock.Indent(2);

  for (auto &cls: translatedProject.mClasses) {
    codeBlock.Add("gen/{}.cpp", cls.mName);
  }
  codeBlock.Indent(-2);
  codeBlock.Add(")");
  codeBlock.Add("set(src_sources");
  codeBlock.Indent(2);

  for (auto &cls: translatedProject.mClasses) {
    if (HasUserDefinedMethods(cls)) {
      codeBlock.Add("src/{}.cpp", cls.mName);
    }
  }
  codeBlock.Indent(-2);
  codeBlock.Add(")");

  codeBlock.Add("set(custom_sources)");
  codeBlock.UserDefined("CustomSources");
  codeBlock.Add("add_library({} STATIC ${{gen_sources}} ${{src_sources}} ${{custom_sources}})",
                mGeneratorSettings.mProjectName);
  codeBlock.UserDefined("CustomDependencies");
  // TODO: complete lua generator, fix unused parameters and enable this
  codeBlock.Add("if (UNIX)");
  codeBlock.Indent(1);
  codeBlock.Add("target_compile_options({} PRIVATE -Wall -Wextra -Wpedantic -Werror "
                "-Wno-unused-parameter -Wno-unused-variable)",
                mGeneratorSettings.mProjectName);
  codeBlock.Indent(-1);
  codeBlock.Add("endif ()");
  cmake.mBody = std::move(codeBlock);
}

HeaderContainer CodeGenerator::PrepareIncludes(const Class &cls, bool isHeader) const {
  HeaderContainer headers;
  if (isHeader)
    headers = cls.mHeaderIncludes;
  else
    headers = cls.mSourceIncludes;

  for (const auto &field: cls.mFields) {
    headers.IncludeClassField(*mTranslatedProject, cls, field, isHeader);
  }
  for (const auto &method: cls.mMethods) {
    headers.IncludeClassMethod(*mTranslatedProject, cls, method, isHeader);
  }
  for (const auto &usingStatement: cls.mUsings) {
    headers.IncludeUsing(*mTranslatedProject, usingStatement, isHeader);
  }
  return headers;
}

void CodeGenerator::GenerateClassDeclarationsForHeader(
    CodeBlock &codeBlock HOLGEN_ATTRIBUTE_UNUSED, const Class &cls HOLGEN_ATTRIBUTE_UNUSED) const {}

void CodeGenerator::GenerateHolgenHeader(GeneratedContent &header) const {
  header.mType = FileType::CppHeader;
  header.mName = "holgen.h";
  CodeBlock codeBlock;
  codeBlock.Add("// {}", GenMessage);
  codeBlock.Add("#pragma once");
  codeBlock.Line();
  codeBlock.Add("#include <iostream>");
  codeBlock.Add("#include <format>");
  if (!mGeneratorSettings.mConfigHeader.empty()) {
    codeBlock.Add("#include \"{}\"", mGeneratorSettings.mConfigHeader);
  }

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_FAIL");
  codeBlock.Add(
      R"(#define HOLGEN_FAIL(msg, ...) throw std::runtime_error(std::format("{{}}:{{}} " msg, __FILE__, __LINE__, ## __VA_ARGS__)))");
  codeBlock.Add("#endif // ifndef HOLGEN_FAIL");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_FAIL_IF");
  codeBlock.Add("#define HOLGEN_FAIL_IF(cond, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_FAIL(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_FAIL_IF");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN");
  codeBlock.Add(
      R"(#define HOLGEN_WARN(msg, ...) std::cerr << std::format("{{}}:{{}} " msg, __FILE__, __LINE__, ## __VA_ARGS__) << std::endl)");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN_IF");
  codeBlock.Add("#define HOLGEN_WARN_IF(cond, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN_IF");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN_AND_RETURN_IF");
  codeBlock.Add("#define HOLGEN_WARN_AND_RETURN_IF(cond, retVal, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("return retVal; \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN_AND_RETURN_IF");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN_AND_CONTINUE_IF");
  codeBlock.Add("#define HOLGEN_WARN_AND_CONTINUE_IF(cond, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("continue; \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN_AND_CONTINUE_IF");

  header.mBody = std::move(codeBlock);
}

void CodeGenerator::GenerateUsingsForHeader(CodeBlock &codeBlock, const Class &cls) const {
  for (auto &usingStatement: cls.mUsings) {
    codeBlock.Add("using {}={};", usingStatement.mTargetType,
                  usingStatement.mSourceType.ToString(true));
  }
}

CodeBlock CodeGenerator::GenerateConstructorsForSource(const Class &cls) const {
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

std::string CodeGenerator::GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
                                                     bool isInHeader, bool isInsideClass) const {
  std::stringstream ss;
  if (method.mConstexprness == Constexprness::Constexpr)
    ss << "constexpr ";
  if (method.mStaticness == Staticness::Static && isInsideClass && isInHeader)
    ss << "static ";
  if (!isInHeader && cls.GetUsing(method.mReturnType.mName))
    ss << cls.mName << "::";
  ss << method.mReturnType.ToString(false);
  if (!isInHeader || !isInsideClass)
    ss << cls.mName << "::";
  ss << method.mName << "(";
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

std::string CodeGenerator::GenerateFunctionSignature(const Class &cls, const ClassConstructor &ctor,
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

CodeBlock CodeGenerator::GenerateDestructor(const Class &cls, Visibility visibility,
                                            bool isHeader) const {
  CodeBlock codeBlock;
  if (cls.mDestructor.IsEmpty() || (isHeader && cls.mDestructor.mVisibility != visibility))
    return {};
  bool definedInHeader = CanBeDefinedInHeader(cls, cls.mDestructor);
  if (isHeader) {
    if (!definedInHeader) {
      codeBlock.Add("~{}();", cls.mName);
    } else if (cls.mDestructor.mDefaultDelete == DefaultDelete::Default) {
      codeBlock.Add("~{}() = default;", cls.mName);
    } else if (cls.mDestructor.mDefaultDelete == DefaultDelete::Delete) {
      codeBlock.Add("~{}() = delete;", cls.mName);
    } else {
      codeBlock.Add("~{}() {{", cls.mName);
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

void CodeGenerator::GenerateClassModifiableSource(GeneratedContent &source,
                                                  const Class &cls) const {
  source.mType = FileType::CppSource;
  source.mName = std::format("src/{}.cpp", cls.mName);
  CodeBlock codeBlock;
  codeBlock.Add("// {}", PartialGenMessage);
  codeBlock.Add("#include \"../gen/{}.h\"", cls.mName);
  codeBlock.UserDefined("{}_CustomIncludes", cls.mName);
  codeBlock.Line();
  if (!cls.mNamespace.empty())
    codeBlock.Add("namespace {} {{", cls.mNamespace);
  for (auto &method: cls.mMethods) {
    if (!method.mUserDefined)
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

void CodeGenerator::GenerateNestedEnumsForHeader(CodeBlock &codeBlock, const Class &cls,
                                                 Visibility visibility) const {
  for (auto &nestedEnum: cls.mNestedEnums) {
    if (nestedEnum.mVisibility != visibility)
      continue;
    AddComments(codeBlock, nestedEnum.mComments);
    if (nestedEnum.mUnderlyingType.empty())
      codeBlock.Add("enum {} {{", nestedEnum.mName);
    else
      codeBlock.Add("enum {} : {} {{", nestedEnum.mName, nestedEnum.mUnderlyingType);
    codeBlock.Indent(1);
    for (auto &entry: nestedEnum.mEntries) {
      AddComments(codeBlock, entry.mComments);
      if (entry.mValue.empty())
        codeBlock.Add("{},", entry.mName);
      else
        codeBlock.Add("{} = {},", entry.mName, entry.mValue);
    }
    codeBlock.Indent(-1);
    codeBlock.Add("}};");
  }
}
} // namespace holgen
