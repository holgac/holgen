#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include "core/Exception.h"
#include "generator/CodeGenerator.h"
#include "generator/Translator.h"
#include "generator/UserDefinedSectionExtractor.h"
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"
#include "cargs.h"

using namespace holgen;

namespace {
// TODO: this is needed by dataManager's ParseFiles too. Move to FileSystemHelper?
std::string ReadFile(const std::filesystem::path &path) {
  std::ifstream fin(path, std::ios_base::binary);
  fin.seekg(0, std::ios_base::end);
  std::string contents(fin.tellg(), 0);
  fin.seekg(0, std::ios_base::beg);
  fin.read(contents.data(), contents.size());
  return contents;
}

struct CliOptions {
  std::vector<std::string> mSchemaDirs;
  std::string mOutDir;
  std::string mNamespace;
  std::string mCmakeTarget;
  std::string mConfigHeader;
};

bool ParseArgs(CliOptions &out, int argc, char **argv) {
  cag_option cargsCliOptions[] = {
      {.identifier = 'i',
       .access_letters = "i",
       .access_name = "input",
       .value_name = "DIR",
       .description = "Directory containing holgen schema files"},
      {.identifier = 'o',
       .access_letters = "o",
       .access_name = "cpp_output",
       .value_name = "DIR",
       .description = "Directory to put the C++ output files"},
      {.identifier = 'n',
       .access_letters = "n",
       .access_name = "namespace",
       .value_name = "VALUE",
       .description = "C++ namespace to use for all data structures"},
      {.identifier = 'c',
       .access_letters = "c",
       .access_name = "cmake",
       .value_name = "VALUE",
       .description = "CMake target name to use for the generated static lib"},
      {.identifier = 'h',
       .access_letters = "h",
       .access_name = "header",
       .value_name = "VALUE",
       .description = "Config header to include in all generated headers"},
  };
  cag_option_context context;
  cag_option_init(&context, cargsCliOptions, std::size(cargsCliOptions), argc, argv);
  while (cag_option_fetch(&context)) {
    switch (cag_option_get_identifier(&context)) {
    case 'i':
      out.mSchemaDirs.emplace_back(cag_option_get_value(&context));
      break;
    case 'o':
      out.mOutDir = cag_option_get_value(&context);
      break;
    case 'n':
      out.mNamespace = cag_option_get_value(&context);
      break;
    case 'c':
      out.mCmakeTarget = cag_option_get_value(&context);
      break;
    case 'h':
      out.mConfigHeader = cag_option_get_value(&context);
      break;
    default:
    case '?':
      cag_option_print_error(&context, stdout);
      cag_option_print(cargsCliOptions, std::size(cargsCliOptions), stdout);
      return false;
    }
  }
  if (out.mOutDir.empty() || out.mSchemaDirs.empty() || out.mNamespace.empty() ||
      out.mCmakeTarget.empty() || out.mConfigHeader.empty()) {
    cag_option_print(cargsCliOptions, std::size(cargsCliOptions), stdout);
    return false;
  }
  return true;
}

ProjectDefinition ParseProjectDefinition(const CliOptions &cliOptions) {
  ProjectDefinition projectDefinition;
  std::queue<std::filesystem::path> pathsQueue;
  for (auto &path: cliOptions.mSchemaDirs) {
    pathsQueue.push(path);
  }

  while (!pathsQueue.empty()) {
    auto &curPath = pathsQueue.front();
    for (auto &entry: std::filesystem::directory_iterator(std::filesystem::path(curPath))) {
      if (std::filesystem::is_directory(entry)) {
        pathsQueue.push(entry.path());
        continue;
      }
      if (!std::filesystem::is_regular_file(entry) || entry.path().extension() != ".hsc") {
        continue;
      }
      auto contents = ReadFile(entry.path());
      Tokenizer tokenizer(contents, entry.path().string());
      try {
        Parser{projectDefinition, tokenizer}.Parse();
      } catch (Exception &exc) {
        std::cerr << "In file " << entry.path() << std::endl;
        throw;
      }
    }
    pathsQueue.pop();
  }
  Parser::PostProcess(projectDefinition);
  return projectDefinition;
}

int Run(const CliOptions &cliOptions) {
  auto projectDefinition = ParseProjectDefinition(cliOptions);

  TranslatorSettings translatorSettings{cliOptions.mNamespace};
  Translator translator{translatorSettings};
  auto project = translator.Translate(projectDefinition);
  auto generator = CodeGenerator({cliOptions.mCmakeTarget, cliOptions.mConfigHeader});
  auto results = generator.Generate(project);
  std::filesystem::path outDir(cliOptions.mOutDir);
  for (auto &result: results) {
    auto target = outDir / result.mName;
    auto dirname = target.parent_path();
    if (!std::filesystem::exists(dirname)) {
      std::filesystem::create_directories(dirname);
    }
    std::map<std::string, std::string> sections;
    std::string existingContents;
    if (std::filesystem::exists(target)) {
      existingContents = ReadFile(target);
      sections = UserDefinedSectionExtractor().Extract(existingContents);
    }
    auto newContents = result.mBody.ToString(result.mType, sections);
    if (newContents == existingContents)
      continue;
    std::ofstream fout(target, std::ios::binary);
    fout.write(newContents.c_str(), newContents.size());
  }
  // TODO: warn if there are files in the directory not created by us in case the schema changed
  // but the dangling files weren't deleted.
  return 0;
}
} // namespace

int main(int argc, char **argv) {
  try {
    CliOptions cliOptions;
    if (!ParseArgs(cliOptions, argc, argv)) {
      return -1;
    }
    return Run(cliOptions);
  } catch (Exception &exc) {
    std::cerr << exc.what() << std::endl;
    return -1;
  }
}
