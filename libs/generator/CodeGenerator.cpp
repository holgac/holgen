#include "CodeGenerator.h"
#include <sstream>
#include <core/Decorators.h>

#include "HeaderContainer.h"

namespace holgen {

  namespace {
    std::string StringifyTemplateParameters(const std::vector<TemplateParameter> &templateParameters) {
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
  }

  std::vector<GeneratedContent> CodeGenerator::Generate(const TranslatedProject &translatedProject) {
    mTranslatedProject = &translatedProject;
    std::vector<GeneratedContent> contents;
    for (auto &cls: translatedProject.mClasses) {
      GenerateClassHeader(contents.emplace_back(), cls);
      GenerateClassSource(contents.emplace_back(), cls);
    }
    GenerateHolgenHeader(contents.emplace_back());
    GenerateCMakeLists(contents.emplace_back(), translatedProject);

    mTranslatedProject = nullptr;

    return contents;
  }

  void CodeGenerator::GenerateClassHeader(GeneratedContent &header, const Class &cls) const {
    header.mType = FileType::CppHeader;
    header.mName = cls.mName + ".h";
    CodeBlock codeBlock;
    codeBlock.Line() << "#pragma once";
    codeBlock.Line();
    codeBlock.Add("#include \"holgen.h\"");
    GenerateIncludes(codeBlock, cls, true);
    codeBlock.Add(cls.mGlobalForwardDeclarations);
    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "namespace " << mGeneratorSettings.mNamespace << " {";

    GenerateClassDeclarationsForHeader(codeBlock, cls);

    if (!cls.mTemplateParameters.empty())
      codeBlock.AddLine(StringifyTemplateParameters(cls.mTemplateParameters));

    // TODO: struct-specific namespaces defined via decorators
    codeBlock.Line() << "class " << cls.mName << " {";

    GenerateForVisibility(codeBlock, cls, Visibility::Public);
    GenerateForVisibility(codeBlock, cls, Visibility::Protected);
    GenerateForVisibility(codeBlock, cls, Visibility::Private);

    codeBlock.Line() << "};"; // class
    GenerateMethodsForHeader(codeBlock, cls, Visibility::Public, false);
    GenerateMethodsForHeader(codeBlock, cls, Visibility::Protected, false);
    GenerateMethodsForHeader(codeBlock, cls, Visibility::Private, false);
    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "}"; // namespace
    header.mText = codeBlock.ToString();
  }

  void CodeGenerator::GenerateForVisibility(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const {
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
    GenerateMethodsForHeader(codeBlock, cls, visibility, true);
    GenerateFieldDeclarations(codeBlock, cls, visibility);
    codeBlock.Indent(-1);
  }

  void CodeGenerator::GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const {
    for (auto &field: cls.mFields) {
      if (field.mVisibility != visibility)
        continue;
      {
        auto line = codeBlock.Line();
        if (field.mIsStatic)
          line << "inline static ";
        line << field.mType.ToString() << " " << field.mName;

        if (!field.mDefaultValue.empty())
          line << " = " << field.mDefaultValue;
        line << ";";
      }
    }
  }

  void CodeGenerator::GenerateMethodsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility,
                                               bool isInsideClass) const {
    for (auto &method: cls.mMethods) {
      if (method.mVisibility != visibility)
        continue;
      if (isInsideClass && method.mIsTemplateSpecialization)
        continue;
      if (!isInsideClass && !method.mIsTemplateSpecialization)
        continue;

      if (!method.mTemplateParameters.empty())
        codeBlock.AddLine(StringifyTemplateParameters(method.mTemplateParameters));
      if (method.mIsTemplateSpecialization)
        codeBlock.Line() << "template <>";

      bool willDefine = !method.mTemplateParameters.empty() || !cls.mTemplateParameters.empty();

      {
        auto line = codeBlock.Line();
        if (method.mIsStatic && isInsideClass)
          line << "static ";
        line << method.mReturnType.ToString() << " ";
        if (!isInsideClass)
          line << cls.mName << "::";
        line << method.mName << "(";
        bool isFirst = true;
        for (auto &arg: method.mArguments) {
          if (!isFirst)
            line << ", ";
          else
            isFirst = false;
          line << arg.mType.ToString() << " " << arg.mName;
        }
        line << ")";
        if (method.mIsConst)
          line << " const";
        if (!willDefine) {
          line << ";";
          continue;
        } else {
          line << " {";
        }
      }
      codeBlock.Indent(1);
      codeBlock.Add(method.mBody);
      codeBlock.Indent(-1);
      codeBlock.Line() << "}";
    }
  }

  void CodeGenerator::GenerateClassSource(GeneratedContent &source, const Class &cls) const {
    source.mType = FileType::CppSource;
    source.mName = cls.mName + ".cpp";
    CodeBlock codeBlock;
    codeBlock.Line() << "#include \"" << cls.mName << ".h\"";
    codeBlock.Line();
    GenerateIncludes(codeBlock, cls, false);

    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "namespace " << mGeneratorSettings.mNamespace << " {";
    // TODO: struct-specific namespaces defined via decorators
    GenerateMethodsForSource(codeBlock, cls);
    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "}"; // namespace

    source.mText = codeBlock.ToString();
  }

  void CodeGenerator::GenerateMethodsForSource(CodeBlock &codeBlock, const Class &cls) const {
    if (!cls.mTemplateParameters.empty())
      return;

    for (auto &method: cls.mMethods) {
      if (!method.mTemplateParameters.empty()) {
        // These are defined in the header
        continue;
      }
      if (method.mIsTemplateSpecialization) {
        codeBlock.Line() << "template <>";
      }
      {
        auto line = codeBlock.Line();
        line << method.mReturnType.ToString() << " " << cls.mName << "::" << method.mName << "(";
        bool isFirst = true;
        for (auto &arg: method.mArguments) {
          if (!isFirst)
            line << ", ";
          else
            isFirst = false;
          line << arg.mType.ToString() << " " << arg.mName;
        }
        line << ")";
        if (method.mIsConst)
          line << " const";
        line << " {";
      }
      codeBlock.Indent(1);
      codeBlock.Add(method.mBody);
      codeBlock.Indent(-1);
      codeBlock.Line() << "}";
    }
  }

  void CodeGenerator::GenerateCMakeLists(GeneratedContent &cmake, const TranslatedProject &translatedProject) const {
    cmake.mType = FileType::CMakeFile;
    cmake.mName = "CMakeLists.txt";
    CodeBlock codeBlock;
    {
      auto line = codeBlock.Line();
      line << "add_library(" << mGeneratorSettings.mCMakeTarget;
      for (auto &cls: translatedProject.mClasses) {
        line << " " << cls.mName << ".cpp";
      }
      line << ")";
    }
    // TODO: complete lua generator, fix unused parameters and enable this
    // codeBlock.Line() << "target_compile_options(" << mGeneratorSettings.mCMakeTarget << " PRIVATE -Wall -Wextra -Wpedantic -Werror)";
    cmake.mText = codeBlock.ToString();
  }

  void CodeGenerator::GenerateIncludes(CodeBlock &codeBlock, const Class &cls, bool isHeader) const {
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

    headers.Write(codeBlock);
  }

  void CodeGenerator::GenerateClassDeclarationsForHeader(CodeBlock &_codeBlock __attribute__((unused)),
                                                         const Class &_cls __attribute__((unused))) const {
  }

  void CodeGenerator::GenerateHolgenHeader(GeneratedContent &header) const {
    header.mType = FileType::CppHeader;
    header.mName = "holgen.h";
    CodeBlock codeBlock;
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
        R"(#define HOLGEN_WARN(msg, ...) std::cerr << std::format("{{}}:{{}} " msg, __FILE__, __LINE__, ## __VA_ARGS__))");
    codeBlock.Add("#endif // ifndef HOLGEN_WARN");

    codeBlock.Line();
    codeBlock.Add("#ifndef HOLGEN_WARN_IF");
    codeBlock.Add("#define HOLGEN_WARN_IF(cond, msg, ...) if (cond) {{ \\");
    codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
    codeBlock.Add("}}");
    codeBlock.Add("#endif // ifndef HOLGEN_WARN_IF");

    header.mText = codeBlock.ToString();
  }

}
