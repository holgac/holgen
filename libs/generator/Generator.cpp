#include "Generator.h"
#include <sstream>
#include <set>

namespace holgen {

  namespace {
    // TODO: overload operator<< instead
    template<typename T>
    void Stringify(T &ss, const Type &type) {
      if (type.mIsConst)
        ss << "const ";
      ss << type.mName;
      if (type.mType == TypeType::Reference)
        ss << "&";
      else if (type.mType == TypeType::Pointer)
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
    std::set<std::string> CstdIntTypes = {
        {"int8_t"},
        {"int16_t"},
        {"int32_t"},
        {"int64_t"},
        {"uint8_t"},
        {"uint16_t"},
        {"uint32_t"},
        {"uint64_t"},
          };
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
    GenerateHeadersForHeader(codeBlock, cls);
    codeBlock.Line();
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
    GenerateMethodDeclarations(codeBlock, cls, visibility);
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

  void Generator::GenerateMethodDeclarations(CodeBlock &codeBlock, const Class &cls, Visibility visibility) const {
    for (auto &method: cls.mMethods) {
      if (method.mVisibility != visibility)
        continue;
      auto line = codeBlock.Line();
      Stringify(line, method.mType);
      line << " " << method.mName << "(";
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
      line << ";";
    }
  }

  void Generator::GenerateClassSource(GeneratedContent &source, const Class &cls) const {
    source.mType = FileType::CppSource;
    source.mName = cls.mName + ".cpp";
    CodeBlock codeBlock;
    codeBlock.Line() << "#include \"" << cls.mName << ".h\"";
    codeBlock.Line();
    GenerateHeadersForSource(codeBlock, cls);
    // TODO: include headers
    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "namespace " << mGeneratorSettings.mNamespace << " {";
    // TODO: struct-specific namespaces defined via decorators
    GenerateMethodDefinitions(codeBlock, cls);
    if (!mGeneratorSettings.mNamespace.empty())
      codeBlock.Line() << "}"; // namespace

    source.mText = ToString(codeBlock);
  }

  void Generator::GenerateMethodDefinitions(CodeBlock &codeBlock, const Class &cls) const {
    for (auto &method: cls.mMethods) {
      {
        auto line = codeBlock.Line();
        Stringify(line, method.mType);
        line << " " <<  cls.mName << "::" << method.mName << "(";
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
      for(auto& cls: translatedProject.mClasses) {
        line << " " << cls.mName << ".cpp";
      }
      line << ")";
    }
    cmake.mText = ToString(codeBlock);
  }

  void Generator::GenerateHeadersForHeader(CodeBlock &codeBlock, const Class &cls) const {
    // First include standard libs
    std::set<std::string> includedHeaders;
    for(const auto& field: cls.mFields) {
      std::string header;
      if (CstdIntTypes.contains(field.mType.mName)) {
        header = "<cstdint>";
      } else {
        continue;
      }
      if (!includedHeaders.contains(header)) {
        codeBlock.Line() << "#include " << header;
        includedHeaders.insert(header);
      }
    }

    // Then include
  }

  void Generator::GenerateClassDeclarationsForHeader(CodeBlock &codeBlock, const Class &cls) const {
  }

  void Generator::GenerateHeadersForSource(CodeBlock &codeBlock, const Class &cls) const {
  }

}
