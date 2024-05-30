#include "CodeBlock.h"
#include <set>
#include "core/Exception.h"
#include "core/St.h"

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
    FileType fileType, const std::map<std::string, std::string> &sections
) const {
  std::set<std::string> usedSections;
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
        out << indentation << *lineIt << St::Newline;
        ++lineIt;
        break;
      case CodeUnitType::UserDefined:
        out << commentStart << " " << St::UserDefinedSectionBegin << *lineIt << St::Newline;
        auto sectionIt = sections.find(*lineIt);
        if (sectionIt != sections.end()) {
          out << sectionIt->second;
          usedSections.insert(*lineIt);
        }
        out << commentStart << " " << St::UserDefinedSectionEnd << *lineIt << St::Newline;
        ++lineIt;
        break;
    }
  }
  THROW_IF(currentIndentation != 0, "Inconsistent indentation!")
  for (auto &[sectionName, sectionContent]: sections) {
    THROW_IF(
        !usedSections.contains(sectionName) && !sectionContent.empty(),
        "Section {} was removed since the last run. Proceeding would erase the content. "
        "Please manually remove the section and rerun the generator.",
        sectionName);
  }
  return out.str();
}

void CodeBlock::AddUserDefinedSection(const std::string &name) {
  mContents.push_back(CodeUnitType::UserDefined);
  mLines.push_back(name);
}
}
