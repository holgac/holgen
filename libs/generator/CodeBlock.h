#pragma once
#include <vector>
#include <cstdint>
#include <string>

#include "core/LineWithAction.h"


namespace holgen {
  enum class CodeUnitType {
    Code,
    Indentation,
  };

  struct CodeBlock {
    std::vector <CodeUnitType> mContents;
    std::vector <std::string> mLines;
    std::vector <ssize_t> mIndentations;

    void Indent(ssize_t amount) {
      mContents.push_back(CodeUnitType::Indentation);
      mIndentations.push_back(amount);
    }

    LineWithAction Line() {
      return {[this](const auto &s) -> void { this->AddLine(s); }};
    }

    void Add(const CodeBlock &codeBlock);


    void AddLine(const std::string &line) {
      mContents.push_back(CodeUnitType::Code);
      mLines.push_back(line);
    }

    std::string ToString() const;
  };

}
