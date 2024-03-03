#include "HeaderContainer.h"
#include <map>

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
        {"std::deque",        "deque"},
        {"std::map",           "map"},
        {"std::unordered_map", "unordered_map"},
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

  void HeaderContainer::IncludeType(const Type &type, bool isHeader) {
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
    } else if (type.mName.starts_with("rapidjson::")) {
      // it's better to fwd declare in header, but rapidjson uses templates making it complicated
      if (isHeader) {
        AddLibHeader("rapidjson/document.h");
      }
    } else if (type.mName.starts_with("lua_State")) {
      if (isHeader) {
        // TODO: implement fwd declaring
        AddLibHeader("lua.hpp");
      }
    } else {
      // TODO: This will change when we implement fwd declarations
      if (isHeader) {
        AddLocalHeader(type.mName + ".h");
      }
    }
  }

  void HeaderContainer::IncludeClassField(const Class &cls, const ClassField &classField, bool isHeader) {
    IncludeClassField(cls, classField, classField.mType, isHeader);
  }

  void HeaderContainer::IncludeClassField(
      const Class &cls, const ClassField &classField, const Type &type, bool isHeader
  ) {
    bool isTemplateType = false;
    for (const auto &templateParameter: cls.mTemplateParameters) {
      if (templateParameter.mName == type.mName) {
        isTemplateType = true;
        break;
      }
    }
    if (!isTemplateType)
      IncludeType(type, isHeader);
    for (const auto &templateParameter: type.mTemplateParameters) {
      IncludeClassField(cls, classField, templateParameter, isHeader);
    }
    for (const auto &templateParameter: type.mFunctionalTemplateParameters) {
      IncludeClassField(cls, classField, templateParameter, isHeader);
    }
  }

  void HeaderContainer::IncludeClassMethod(const Class &cls, const ClassMethod &classMethod, bool isHeader) {
    IncludeClassMethod(cls, classMethod, classMethod.mReturnType, isHeader);
    for (const auto &argument: classMethod.mArguments) {
      IncludeClassMethod(cls, classMethod, argument.mType, isHeader);
    }
  }

  void HeaderContainer::IncludeClassMethod(const Class &cls, const ClassMethod &classMethod, const Type &type,
                                           bool isHeader) {
    bool isTemplateType = false;
    for (const auto &templateParameter: cls.mTemplateParameters) {
      if (templateParameter.mName == type.mName) {
        isTemplateType = true;
        break;
      }
    }
    for (const auto &templateParameter: classMethod.mTemplateParameters) {
      if (templateParameter.mName == type.mName) {
        isTemplateType = true;
        break;
      }
    }
    if (!isTemplateType && type.mName != cls.mName)
      IncludeType(type, isHeader);
    for (const auto &templateParameter: type.mTemplateParameters) {
      IncludeClassMethod(cls, classMethod, templateParameter, isHeader);
    }
    for (const auto &templateParameter: type.mFunctionalTemplateParameters) {
      IncludeClassMethod(cls, classMethod, templateParameter, isHeader);
    }
  }
}
