#include "UserDefinedSectionExtractor.h"
#include "FileType.h"
#include "core/St.h"
#include "core/Exception.h"

namespace holgen {

  std::map<std::string, std::string>
  UserDefinedSectionExtractor::Extract( const std::string &fileContents) const {
    size_t idx = 0;
    std::map<std::string, std::string> sections;
    while (true) {
      idx = fileContents.find(St::UserDefinedSectionBegin, idx);
      if (idx == std::string::npos) {
        break;
      }
      idx += St::UserDefinedSectionBegin.size();
      auto nextNewLine = fileContents.find('\n', idx);
      auto sectionName = fileContents.substr(idx, nextNewLine - idx);
      THROW_IF(nextNewLine == std::string::npos, "Invalid section name!");
      auto end = fileContents.find(St::UserDefinedSectionEnd + sectionName + "\n", nextNewLine);
      THROW_IF(end == std::string::npos, "Unterminated user defined section {}", sectionName);
      end = fileContents.rfind('\n', end);
      auto sectionContent = fileContents.substr(nextNewLine + 1, end - nextNewLine);
      sections.emplace(std::move(sectionName), std::move(sectionContent));
      idx = end + St::UserDefinedSectionEnd.size() + 1;
    }

    return sections;
  }
}
