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
  if (argc != 6) {
    std::cerr << "Usage: " << std::endl
              << argv[0]
              << " [IN_DIRECTORY] [OUT_DIRECTORY] [NAMESPACE] [CMAKE_TARGET] [CONFIG_HEADER]"
              << std::endl;
    return false;
  }
  out.mSchemaDirs = {argv[1]};
  out.mOutDir = argv[2];
  out.mNamespace = argv[3];
  out.mCmakeTarget = argv[4];
  out.mConfigHeader = argv[5];
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
  // TODO: warn if there are files in the directory not created by us in case the schema changed but
  // the dangling files weren't deleted.
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
