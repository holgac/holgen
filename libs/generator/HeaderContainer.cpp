#include "HeaderContainer.h"
#include <map>
#include "Translator.h"

namespace holgen {
  namespace {
    std::set<std::string> CstdIntTypes = {
        "int8_t",
        "int16_t",
        "int32_t",
        "int64_t",
        "uint8_t",
        "uint16_t",
        "uint32_t",
        "uint64_t",
    };
    std::map<std::string, std::string> STDHeaders = {
        {"std::string",        "string"},
        {"std::vector",        "vector"},
        {"std::deque",         "deque"},
        {"std::map",           "map"},
        {"std::unordered_map", "unordered_map"},
        {"std::set",           "set"},
        {"std::unordered_set", "unordered_set"},
        {"std::function",      "functional"},
    };
    std::set<std::string> NoHeaderTypes = {"float", "double", "void", "bool"};
  }

  void HeaderContainer::AddStandardHeader(const std::string &header) {
    if (mHeaders.contains(header))
      return;
    mHeaders.insert(header);
    mStandardHeaders.push_back(header);
  }

  void HeaderContainer::AddLibHeader(const std::string &header) {
    if (mHeaders.contains(header))
      return;
    mHeaders.insert(header);
    mLibHeaders.push_back(header);
  }

  void HeaderContainer::AddLocalHeader(const std::string &header) {
    if (mHeaders.contains(header))
      return;
    mHeaders.insert(header);
    mLocalHeaders.push_back(header);
  }

  void HeaderContainer::Write(CodeBlock &codeBlock) {
    for (const auto &header: mStandardHeaders)
      codeBlock.Line() << "#include <" << header << ">";
    for (const auto &header: mLibHeaders)
      codeBlock.Line() << "#include <" << header << ">";
    for (const auto &header: mLocalHeaders)
      codeBlock.Line() << "#include \"" << header << "\"";
    if (!mHeaders.empty())
      codeBlock.Line();
  }

  void HeaderContainer::IncludeType(const TranslatedProject &project, const Type &type, bool isHeader) {
    if (NoHeaderTypes.contains(type.mName)) {
      return;
    } else if (CstdIntTypes.contains(type.mName)) {
      if (isHeader) {
        AddStandardHeader("cstdint");
      }
    } else if (STDHeaders.contains(type.mName)) {
      if (isHeader) {
        AddStandardHeader(STDHeaders.at(type.mName));
      }
    } else if (project.GetClass(type.mName) != nullptr) {
      if (isHeader) {
        AddLocalHeader(type.mName + ".h");
      }
    }
  }

  void HeaderContainer::IncludeClassField(
      const TranslatedProject &project, const Class &cls, const ClassField &classField, bool isHeader
  ) {
    IncludeClassField(project, cls, classField, classField.mType, isHeader);
  }

  void HeaderContainer::IncludeClassField(const TranslatedProject &project,
                                          const Class &cls, const ClassField &classField, const Type &type,
                                          bool isHeader
  ) {
    bool isLocalType = false;
    for (const auto &templateParameter: cls.mTemplateParameters) {
      if (templateParameter.mName == type.mName) {
        isLocalType = true;
        break;
      }
    }
    for (const auto &usingStatement : cls.mUsings) {
      if (usingStatement.mTargetType == type.mName) {
        isLocalType = true;
        break;
      }
    }
    if (!isLocalType)
      IncludeType(project, type, isHeader);
    for (const auto &templateParameter: type.mTemplateParameters) {
      IncludeClassField(project, cls, classField, templateParameter, isHeader);
    }
    for (const auto &templateParameter: type.mFunctionalTemplateParameters) {
      IncludeClassField(project, cls, classField, templateParameter, isHeader);
    }
  }

  void HeaderContainer::IncludeClassMethod(const TranslatedProject &project, const Class &cls,
                                           const ClassMethod &classMethod, bool isHeader) {
    IncludeClassMethod(project, cls, classMethod, classMethod.mReturnType, isHeader);
    for (const auto &argument: classMethod.mArguments) {
      IncludeClassMethod(project, cls, classMethod, argument.mType, isHeader);
    }
  }

  void HeaderContainer::IncludeClassMethod(const TranslatedProject &project, const Class &cls,
                                           const ClassMethod &classMethod, const Type &type,
                                           bool isHeader) {
    bool isLocalType = false;
    for (const auto &templateParameter: cls.mTemplateParameters) {
      if (templateParameter.mName == type.mName) {
        isLocalType = true;
        break;
      }
    }
    for (const auto &templateParameter: classMethod.mTemplateParameters) {
      if (templateParameter.mName == type.mName) {
        isLocalType = true;
        break;
      }
    }
    for (const auto &usingStatement : cls.mUsings) {
      if (usingStatement.mTargetType == type.mName) {
        isLocalType = true;
        break;
      }
    }
    // TODO: is this necessary anymore?
    if (!isLocalType && type.mName != cls.mName)
      IncludeType(project, type, isHeader);
    for (const auto &templateParameter: type.mTemplateParameters) {
      IncludeClassMethod(project, cls, classMethod, templateParameter, isHeader);
    }
    for (const auto &templateParameter: type.mFunctionalTemplateParameters) {
      IncludeClassMethod(project, cls, classMethod, templateParameter, isHeader);
    }
  }

  void HeaderContainer::IncludeUsing(const TranslatedProject &project, const Class &cls __attribute__((unused)), const Using &usingStatement,
                                     bool isHeader) {
    IncludeType(project, usingStatement.mSourceType, isHeader);
  }
}
