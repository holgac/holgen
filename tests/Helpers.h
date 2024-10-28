#pragma once

#include <optional>
#include "generator/CodeGenerator.h"

namespace holgen::helpers {
std::map<std::string, GeneratedContent> MapByName(const std::vector<GeneratedContent> &contents);

std::string_view Trim(const std::string_view &str);

void ExpectGeneratedContent(const GeneratedContent &actual, FileType fileType,
                            const std::string &name, const std::string &contents);

void ExpectEqual(const Type &expected, const Type &actual);
void ExpectEqual(const TypeDefinition &expected, const TypeDefinition &actual);
void ExpectEqual(const FunctionReturnTypeDefinition &expected,
                 const FunctionReturnTypeDefinition &actual);
void ExpectEqual(const ClassField &actual, const ClassField &expected);
void ExpectEqual(const ClassEnumEntry &actual, const ClassEnumEntry &expected);
void ExpectEqual(const ClassMethod &actual, const ClassMethod &expected,
                 const std::optional<std::string> &expectedBody = std::nullopt);
void ExpectEqual(const MethodArgument &actual, const MethodArgument &expected);
void ExpectEqual(const ClassConstructor &actual, const ClassConstructor &expected,
                 const std::optional<std::string> &expectedBody = std::nullopt);
void ExpectEqual(const ClassDestructor &actual, const ClassDestructor &expected,
                 const std::optional<std::string> &expectedBody = std::nullopt);
void ExpectEqual(const ClassConstructorInitializer &actual,
                 const ClassConstructorInitializer &expected);
void ExpectEqual(const TemplateParameter &actual, const TemplateParameter &expected);
} // namespace holgen::helpers
