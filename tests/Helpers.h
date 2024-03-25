#pragma once
#include "generator/CodeGenerator.h"

namespace holgen::helpers {
  std::map<std::string, GeneratedContent> MapByName(const std::vector<GeneratedContent> &contents);

  std::string_view Trim(const std::string &str);

  void ExpectGeneratedContent(const GeneratedContent &actual, const GeneratedContent &expected);

  void ExpectTypeEqual(const Type &expected, const Type &actual);

}
