#pragma once

#include <string>
#include <vector>
#include <set>
#include "Translator.h"


namespace holgen {

  class HeaderContainer {
    std::set <std::string> mHeaders;
    std::vector <std::string> mStandardHeaders;
    std::vector <std::string> mLibHeaders;
    std::vector <std::string> mLocalHeaders;
  public:
    void AddStandardHeader(const std::string &header);

    void AddLibHeader(const std::string &header);

    void AddLocalHeader(const std::string &header);

    void Write(CodeBlock &codeBlock);

    void AddForType(const Type &type, bool isHeader);
  };
}