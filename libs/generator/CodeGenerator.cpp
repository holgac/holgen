#include "CodeGenerator.h"

#include <holgen.h>
#include <sstream>
#include "HeaderContainer.h"
#include "generators/CMakeGenerator.h"
#include "generators/HolgenHeaderGenerator.h"
#include "generators/CppModifiableSourceGenerator.h"
#include "generators/CppHeaderGenerator.h"
#include "generators/CppSourceGenerator.h"

namespace holgen {

std::vector<GeneratedContent> CodeGenerator::Generate(const TranslatedProject &translatedProject) {
  mTranslatedProject = &translatedProject;
  std::vector<GeneratedContent> contents;
  CppHeaderGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CppSourceGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CppModifiableSourceGenerator(mGeneratorSettings, translatedProject).Run(contents);
  HolgenHeaderGenerator(mGeneratorSettings, translatedProject).Run(contents);
  CMakeGenerator(mGeneratorSettings, translatedProject).Run(contents);

  if (mGeneratorSettings.IsFeatureEnabled(GeneratorFeatureFlag::SwigMask)) {
    GenerateSwigInterface(contents.emplace_back(), translatedProject);
  }

  mTranslatedProject = nullptr;

  return contents;
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
    line << "};";
  }
  codeBlock.Indent(-1);
  codeBlock.Add("}};");
}

HeaderContainer CodeGenerator::PrepareIncludes(const Class &cls, bool isHeader) const {
  HeaderContainer headers;
  if (isHeader)
    headers = cls.mHeaderIncludes;
  else
    headers = cls.mSourceIncludes;

  if (isHeader) {
    for (auto &baseClass: cls.mBaseClasses) {
      headers.IncludeBaseClass(*mTranslatedProject, cls, baseClass);
    }
  }

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

std::string CodeGenerator::GenerateFunctionSignature(const Class &cls, const ClassMethod &method,
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

std::string CodeGenerator::GenerateFunctionSignature(const CFunction &func, bool isInHeader) const {

  std::stringstream ss;
  if (isInHeader) {
    ss << "HOLGEN_EXPORT ";
  }
  ss << func.mReturnType.ToFullyQualifiedString(*mTranslatedProject);

  ss << func.mName << "(";
  size_t idx = 0;
  for (auto &arg: func.mArguments) {
    if (idx != 0)
      ss << ", ";
    ss << arg.mType.ToFullyQualifiedString(*mTranslatedProject) << arg.mName;
    ++idx;
  }
  ss << ")";
  return ss.str();
}

} // namespace holgen
