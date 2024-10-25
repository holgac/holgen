#include "CMakeGenerator.h"

#include <core/St.h>

namespace holgen {
void CMakeGenerator::Run(std::vector<GeneratedContent> &contents) {
  auto &cmake = contents.emplace_back();
  cmake.mType = FileType::CMakeFile;
  cmake.mName = "CMakeLists.txt";
  CodeBlock codeBlock;
  codeBlock.Add("# {}", St::PartialGenMessage);
  codeBlock.Add("set(gen_sources");
  codeBlock.Indent(2);

  for (auto &cls: mTranslatedProject.mClasses) {
    codeBlock.Add("gen/{}.cpp", cls.mName);
  }
  codeBlock.Indent(-2);
  codeBlock.Add(")");
  codeBlock.Add("set(src_sources");
  codeBlock.Indent(2);

  for (auto &cls: mTranslatedProject.mClasses) {
    if (cls.HasUserDefinedMethods()) {
      codeBlock.Add("src/{}.cpp", cls.mName);
    }
  }
  codeBlock.Indent(-2);
  codeBlock.Add(")");

  codeBlock.Add("set(custom_sources)");
  codeBlock.UserDefined("CustomSources");
  codeBlock.Add("add_library({} STATIC ${{gen_sources}} ${{src_sources}} ${{custom_sources}})",
                mGeneratorSettings.mProjectName);
  codeBlock.UserDefined("CustomDependencies");
  codeBlock.Add("if (UNIX)");
  codeBlock.Indent(1);
  codeBlock.Add("target_compile_options({} PRIVATE -Wall -Wextra -Wpedantic -Werror "
                "-Wno-unused-parameter -Wno-unused-variable)",
                mGeneratorSettings.mProjectName);
  codeBlock.Indent(-1);
  codeBlock.Add("endif ()");
  cmake.mBody = std::move(codeBlock);
}
} // namespace holgen
