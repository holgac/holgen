#include "CodeBlock.h"
#include "core/Exception.h"

namespace holgen {
  void CodeBlock::Add(const CodeBlock &cb) {
    mContents.insert(mContents.end(), cb.mContents.begin(), cb.mContents.end());
    mLines.insert(mLines.end(), cb.mLines.begin(), cb.mLines.end());
    mIndentations.insert(mIndentations.end(), cb.mIndentations.begin(), cb.mIndentations.end());
  }

  void CodeBlock::Add(CodeBlock &&cb) {
    mContents.insert(mContents.end(), std::make_move_iterator(cb.mContents.begin()),
                     std::make_move_iterator(cb.mContents.end()));
    mLines.insert(mLines.end(), std::make_move_iterator(cb.mLines.begin()), std::make_move_iterator(cb.mLines.end()));
    mIndentations.insert(mIndentations.end(), std::make_move_iterator(cb.mIndentations.begin()),
                         std::make_move_iterator(cb.mIndentations.end()));
  }

  std::string CodeBlock::ToString(
      FileType fileType, const std::map<std::string, std::string> &sections __attribute__((unused))
  ) const {
    const char *commentStart = "//";
    if (fileType == FileType::CMakeFile)
      commentStart = "#";
    ssize_t currentIndentation = 0;
    auto contentIt = mContents.begin();
    auto lineIt = mLines.begin();
    auto indentIt = mIndentations.begin();
    std::string indentation;
    std::stringstream out;
    for (; contentIt != mContents.end(); ++contentIt) {
      switch (*contentIt) {
        case CodeUnitType::Indentation:
          currentIndentation += *indentIt;
          THROW_IF(currentIndentation < 0, "Negative indentation!")
          indentation = std::string(currentIndentation * 2, ' ');
          ++indentIt;
          break;
        case CodeUnitType::Code:
          out << indentation << *lineIt << std::endl;
          ++lineIt;
          break;
        case CodeUnitType::UserDefined:
          out << commentStart << " HOLGEN_USER_DEFINED_BEGIN:" << *lineIt << std::endl;
          out << commentStart << " HOLGEN_USER_DEFINED_END:" << *lineIt << std::endl;
          ++lineIt;
          break;
      }
    }
    THROW_IF(currentIndentation != 0, "Inconsistent indentation!")
    return out.str();
  }

  void CodeBlock::AddUserDefinedSection(const std::string &name) {
    mContents.push_back(CodeUnitType::UserDefined);
    mLines.push_back(name);
  }
}
