#pragma once

#include <cstdint>
#include <cstddef>
#include <format>
#include <map>
#include <string>
#include <vector>

#include "FileType.h"
#include "core/LineWithAction.h"

namespace holgen {
enum class CodeUnitType {
  Code,
  Indentation,
  UserDefined,
};

struct CodeBlock {
  std::vector<CodeUnitType> mContents;
  std::vector<std::string> mLines;
  std::vector<ptrdiff_t> mIndentations;

  void AddUserDefinedSection(const std::string &name);

  template <typename... Args>
  void UserDefined(std::format_string<Args...> fmt, Args &&...args) {
    AddUserDefinedSection(std::format(fmt, std::forward<Args>(args)...));
  }

  void Indent(ptrdiff_t amount) {
    mContents.push_back(CodeUnitType::Indentation);
    mIndentations.push_back(amount);
  }

  LineWithAction Line() {
    return {[this](const auto &s) -> void { this->AddLine(s); }};
  }

  void Add(const CodeBlock &codeBlock);
  void Add(CodeBlock &&codeBlock);

  template <typename... Args>
  void Add(std::format_string<Args...> fmt, Args &&...args) {
    AddLine(std::format(fmt, std::forward<Args>(args)...));
  }

  void AddLine(const std::string &line = "") {
    mContents.push_back(CodeUnitType::Code);
    mLines.push_back(line);
  }

  std::string ToString(FileType fileType, const std::map<std::string, std::string> &sections) const;
  bool IsEmpty() const;
};

} // namespace holgen
