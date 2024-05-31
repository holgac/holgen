#pragma once

#include <map>
#include <string>
#include "FileType.h"

namespace holgen {
class UserDefinedSectionExtractor {
public:
  std::map<std::string, std::string> Extract(const std::string &fileContents) const;
};
} // namespace holgen
