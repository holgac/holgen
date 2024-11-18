#include "HolgenHeaderGenerator.h"

#include "core/St.h"

namespace holgen {
void HolgenHeaderGenerator::Run(std::vector<GeneratedContent> &contents) const {
  auto &header = contents.emplace_back();
  header.mType = FileType::CppHeader;
  header.mName = "holgen.h";
  CodeBlock codeBlock;
  codeBlock.Add("// {}", St::GenMessage);
  codeBlock.Add("#pragma once");
  codeBlock.Line();
  codeBlock.Add("#include <iostream>");
  codeBlock.Add("#include <format>");
  if (!mGeneratorSettings.mConfigHeader.empty()) {
    codeBlock.Add("#include \"{}\"", mGeneratorSettings.mConfigHeader);
  }

  codeBlock.Line();
  GenerateErrorCheckMacros(codeBlock);
  GenerateExportMacro(codeBlock);
  GenerateEnumConcept(codeBlock);

  header.mBody = std::move(codeBlock);
}

void HolgenHeaderGenerator::GenerateErrorCheckMacros(CodeBlock &codeBlock) const {

  codeBlock.Add("#ifndef HOLGEN_FAIL");
  codeBlock.Add(
      R"(#define HOLGEN_FAIL(msg, ...) throw std::runtime_error(std::format("{{}}:{{}} " msg, __FILE__, __LINE__, ## __VA_ARGS__)))");
  codeBlock.Add("#endif // ifndef HOLGEN_FAIL");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_FAIL_IF");
  codeBlock.Add("#define HOLGEN_FAIL_IF(cond, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_FAIL(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_FAIL_IF");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN");
  codeBlock.Add(
      R"(#define HOLGEN_WARN(msg, ...) std::cerr << std::format("{{}}:{{}} " msg, __FILE__, __LINE__, ## __VA_ARGS__) << std::endl)");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN_IF");
  codeBlock.Add("#define HOLGEN_WARN_IF(cond, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN_IF");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN_AND_RETURN_IF");
  codeBlock.Add("#define HOLGEN_WARN_AND_RETURN_IF(cond, retVal, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("return retVal; \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN_AND_RETURN_IF");

  codeBlock.Line();
  codeBlock.Add("#ifndef HOLGEN_WARN_AND_CONTINUE_IF");
  codeBlock.Add("#define HOLGEN_WARN_AND_CONTINUE_IF(cond, msg, ...) if (cond) {{ \\");
  codeBlock.Add("HOLGEN_WARN(msg, ## __VA_ARGS__); \\");
  codeBlock.Add("continue; \\");
  codeBlock.Add("}}");
  codeBlock.Add("#endif // ifndef HOLGEN_WARN_AND_CONTINUE_IF");
}

void HolgenHeaderGenerator::GenerateExportMacro(CodeBlock &codeBlock) const {
  codeBlock.Add("#if WINDOWS");
  codeBlock.Add("#define HOLGEN_EXPORT __declspec(dllexport)");
  codeBlock.Add("#elif __GNUC__ >= 4");
  codeBlock.Add("#define HOLGEN_EXPORT __attribute__ ((visibility (\"default\")))");
  codeBlock.Add("#else");
  codeBlock.Add("#define HOLGEN_EXPORT");
  codeBlock.Add("#endif");
}

void HolgenHeaderGenerator::GenerateEnumConcept(CodeBlock &codeBlock) const {
  // Using a random util class here to get the namespace we used for them.
  auto randomUtilClass = mTranslatedProject.GetClass(St::Singleton);
  THROW_IF(!randomUtilClass, "Singleton class was removed!");
  codeBlock.Add("namespace {} {{", randomUtilClass->mNamespace);
  codeBlock.Add("template <typename T>");
  codeBlock.Add("concept {} = requires(T t) {{", St::EnumConcept);
  codeBlock.Indent(1);
  codeBlock.Add("typename T::UnderlyingType;");
  codeBlock.Add("std::is_convertible_v<typename T::UnderlyingType, int64_t>;");
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("T::Invalid");
  codeBlock.Indent(-1);
  codeBlock.Add("}} -> std::convertible_to<int64_t>;");
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("t.GetValue()");
  codeBlock.Indent(-1);
  codeBlock.Add("}} -> std::same_as<typename T::Entry>;");
  codeBlock.Indent(-1);
  codeBlock.Add("}};");
  codeBlock.Add("}}");
}
} // namespace holgen
