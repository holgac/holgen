#pragma once

#include <string>
#include <vector>
#include <set>
#include "Translator.h"


namespace holgen {

  class HeaderContainer {
    std::set<std::string> mHeaders;
    std::vector<std::string> mStandardHeaders;
    std::vector<std::string> mLibHeaders;
    std::vector<std::string> mLocalHeaders;
    void IncludeClassField(const Class &cls,const ClassField &classField, const Type &type, bool isHeader);
    void IncludeClassMethod(const Class &cls, const ClassMethod &classMethod, const Type &type, bool isHeader);
    void IncludeType(const Type &type, bool isHeader);
  public:
    void AddStandardHeader(const std::string &header);

    void AddLibHeader(const std::string &header);

    void AddLocalHeader(const std::string &header);

    void Write(CodeBlock &codeBlock);

    void IncludeClassField(const Class &cls,const ClassField &classField, bool isHeader);
    void IncludeClassMethod(const Class &cls, const ClassMethod &classMethod, bool isHeader);
  };
}