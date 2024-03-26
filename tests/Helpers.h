#pragma once

#include "generator/CodeGenerator.h"
#include <optional>

namespace holgen::helpers {
  std::map<std::string, GeneratedContent> MapByName(const std::vector<GeneratedContent> &contents);

  std::string_view Trim(const std::string &str);

  void ExpectGeneratedContent(const GeneratedContent &actual, const GeneratedContent &expected);

  void ExpectEqual(const Type &expected, const Type &actual);
  void ExpectEqual(const ClassField &actual, const ClassField &expected);
  void ExpectEqual(const ClassMethod &actual, const ClassMethod &expected, const std::optional<std::string>& expectedBody = std::nullopt);
  void ExpectEqual(const ClassMethodArgument &actual, const ClassMethodArgument &expected);
  void ExpectEqual(const ClassConstructor &actual, const ClassConstructor &expected, const std::optional<std::string>& expectedBody = std::nullopt);
  void ExpectEqual(const ClassConstructorInitializer &actual, const ClassConstructorInitializer &expected);
  void ExpectEqual(const TemplateParameter &actual, const TemplateParameter &expected);
}
