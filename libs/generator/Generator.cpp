#include "Generator.h"
#include <sstream>
#include <set>

#include "HeaderContainer.h"

namespace holgen {

  namespace {
    // TODO: overload operator<< instead
    template<typename T>
    void Stringify(T &ss, const Type &type) {
      if (type.mIsConst)
        ss << "const ";
      ss << type.mName;
      if (!type.mTemplateParameters.empty()) {
        ss << "<";
        bool isFirst = true;
        for (const auto &templateParameter: type.mTemplateParameters) {
          if (isFirst) {
            isFirst = false;
          } else {
            ss << ", ";
          }
          Stringify(ss, templateParameter);
        }
        ss << ">";
      }
      if (type.mType == PassByType::Reference)
        ss << "&";
      else if (type.mType == PassByType::Pointer)
        ss << "*";
    }

    std::string ToString(const CodeBlock &codeBlock) {
      ssize_t currentIndentation = 0;
      auto contentIt = codeBlock.mContents.begin();
      auto lineIt = codeBlock.mLines.begin();
      auto indentIt = codeBlock.mIndentations.begin();
      std::string indentation;
      std::stringstream out;
      for (; contentIt != codeBlock.mContents.end(); ++contentIt) {
        switch (*contentIt) {
          case CodeUnitType::Indentation:
            currentIndentation += *indentIt;
            indentation = std::string(currentIndentation * 2, ' ');
            ++indentIt;
            break;
          case CodeUnitType::Code:
            out << indentation << *lineIt << std::endl;
            ++lineIt;
            break;
        }
      }
      if (currentIndentation != 0) {
        throw GeneratorException("Inconsistent indentation!");
      }
      return out.str();
    }
  }

  std::vector<GeneratedContent> Generator::Generate(const TranslatedProject &translatedProject) {
    std::vector<GeneratedContent> contents;
    for (auto &cls: translatedProject.mClasses) {
      GenerateClassHeader(contents.emplace_back(), cls);
      GenerateClassSource(contents.emplace_back(), cls);
    }
    GenerateCMakeLists(contents.emplace_back(), translatedProject);

    return contents;
  }

  void Generator::GenerateClassHeader(GeneratedContent &header, const Class &cls) const {
    header.mType = FileType::CppHeader;
    header.mName = cls.mName + ".h";
    CodeBlock codeBlock;
    codeBlock.Line() << "#pragma once";
    codeBlock.Line();
    GenerateIncludes(codeBlock, cls, true);
    // TODO: include headers
    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "namespace " << mGeneratorSettings.mNamespace << " {";

    GenerateClassDeclarationsForHeader(codeBlock, cls);

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
    header.mText = ToString(codeBlock);
  }

  void Generator::GenerateForVisibility(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const {
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

  void Generator::GenerateFieldDeclarations(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const {
    for (auto &field: cls.mFields) {
      if (field.mVisibility != visibility)
        continue;
      {
        auto line = codeBlock.Line();
        Stringify(line, field.mType);
        line << " " << field.mName << ";";
      }
    }
  }

  void Generator::GenerateMethodsForHeader(CodeBlock &codeBlock, const Class &cls, Visibility visibility,
                                           bool isInsideClass) const {
    for (auto &method: cls.mMethods) {
      if (method.mVisibility != visibility)
        continue;
      if (isInsideClass && method.mIsTemplateSpecialization)
        continue;
      if (!isInsideClass && !method.mIsTemplateSpecialization)
        continue;

      if (!method.mTemplateParameters.empty()) {
        auto templateLine = codeBlock.Line();
        templateLine << "template <";
        bool isFirst = true;
        for (const auto &templateParameter: method.mTemplateParameters) {
          if (isFirst) {
            isFirst = false;
          } else {
            templateLine << ", ";
          }
          templateLine << templateParameter.mType << " " << templateParameter.mName;
        }
        templateLine << ">";
      }
      if (method.mIsTemplateSpecialization) {
        codeBlock.Line() << "template <>";
      }

      {
        auto line = codeBlock.Line();
        if (method.mIsStatic && isInsideClass)
          line << "static ";
        Stringify(line, method.mType);
        line << " ";
        if (!isInsideClass)
          line << cls.mName << "::";
        line << method.mName << "(";
        bool isFirst = true;
        for (auto &arg: method.mArguments) {
          if (!isFirst)
            line << ", ";
          else
            isFirst = false;
          Stringify(line, arg.mType);
          line << " " << arg.mName;
        }
        line << ")";
        if (method.mIsConst)
          line << " const";
        if (method.mTemplateParameters.empty()) {
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

  void Generator::GenerateClassSource(GeneratedContent &source, const Class &cls) const {
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

    source.mText = ToString(codeBlock);
  }

  void Generator::GenerateMethodsForSource(CodeBlock &codeBlock, const Class &cls) const {
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
        Stringify(line, method.mType);
        line << " " << cls.mName << "::" << method.mName << "(";
        bool isFirst = true;
        for (auto &arg: method.mArguments) {
          if (!isFirst)
            line << ", ";
          else
            isFirst = false;
          Stringify(line, arg.mType);
          line << " " << arg.mName;
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

  void Generator::GenerateCMakeLists(GeneratedContent &cmake, const TranslatedProject &translatedProject) const {
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
    cmake.mText = ToString(codeBlock);
  }

  void Generator::GenerateIncludes(CodeBlock &codeBlock, const Class &cls, bool isHeader) const {
    HeaderContainer headers;
    for (const auto &field: cls.mFields) {
      headers.IncludeClassField(field, isHeader);
    }
    for (const auto &method: cls.mMethods) {
      headers.IncludeClassMethod(method, isHeader);

      // TODO: don't hardcode these
      if (!isHeader && method.mName == "ParseJson") {
        // TODO: don't hardcode these
        headers.AddLocalHeader("JsonHelper.h");
      }
    }

    headers.Write(codeBlock);
  }

  void Generator::GenerateClassDeclarationsForHeader(CodeBlock &codeBlock, const Class &cls) const {
  }

}
