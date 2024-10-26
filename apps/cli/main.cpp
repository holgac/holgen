#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include "cargs.h"
#include "core/Exception.h"
#include "generator/CodeGenerator.h"
#include "generator/Translator.h"
#include "generator/UserDefinedSectionExtractor.h"
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"
#include "core/St.h"

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
  std::string mProjectName;
  std::string mConfigHeader;
  bool mLuaEnabled = false;
  bool mJsonEnabled = false;
  std::string mCSharpRoot;
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
      {.identifier = 'p',
       .access_letters = "p",
       .access_name = "project",
       .value_name = "VALUE",
       .description = "Project name to use in cmake static lib"},
      {.identifier = 'h',
       .access_letters = "h",
       .access_name = "header",
       .value_name = "VALUE",
       .description = "Config header to include in all generated headers"},
      {.identifier = 'l',
       .access_letters = nullptr,
       .access_name = "lua",
       .value_name = nullptr,
       .description = "Enable lua bindings (optional, requires lua)"},
      {.identifier = 'c',
       .access_letters = nullptr,
       .access_name = "csharpRoot",
       .value_name = "VALUE",
       .description = "Directory to put the C# output files (enables hostfxr based csharp "
                      "bindings, optional, requires dotnet)"},
      {.identifier = 'j',
       .access_letters = nullptr,
       .access_name = "json",
       .value_name = nullptr,
       .description = "Enable json parsing (optional, requires rapidjson)"},
  };
  cag_option_context context;
  cag_option_init(&context, cargsCliOptions, std::size(cargsCliOptions), argc, argv);
  while (cag_option_fetch(&context)) {
    auto identifier = cag_option_get_identifier(&context);
    switch (identifier) {
    case 'i':
      out.mSchemaDirs.emplace_back(cag_option_get_value(&context));
      break;
    case 'o':
      out.mOutDir = cag_option_get_value(&context);
      break;
    case 'n':
      out.mNamespace = cag_option_get_value(&context);
      break;
    case 'p':
      out.mProjectName = cag_option_get_value(&context);
      break;
    case 'h':
      out.mConfigHeader = cag_option_get_value(&context);
      break;
    case 'l':
      out.mLuaEnabled = true;
      break;
    case 'c':
      out.mCSharpRoot = cag_option_get_value(&context);
      break;
    case 'j':
      out.mJsonEnabled = true;
      break;
    default:
    case '?':
      cag_option_print_error(&context, stdout);
      cag_option_print(cargsCliOptions, std::size(cargsCliOptions), stdout);
      return false;
    }
  }
  if (out.mOutDir.empty() || out.mSchemaDirs.empty() || out.mNamespace.empty() ||
      out.mProjectName.empty() || out.mConfigHeader.empty()) {
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

TranslatorSettings GetTranslatorSettings(const CliOptions &cliOptions) {
  TranslatorSettings translatorSettings{cliOptions.mNamespace};
  if (cliOptions.mLuaEnabled)
    translatorSettings.EnableFeature(TranslatorFeatureFlag::Lua);
  if (cliOptions.mJsonEnabled)
    translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
  return translatorSettings;
}

GeneratorSettings GetGeneratorSettings(const CliOptions &cliOptions) {
  GeneratorSettings generatorSettings{cliOptions.mProjectName, cliOptions.mConfigHeader,
                                      cliOptions.mCSharpRoot};
  return generatorSettings;
}

void WriteToFiles(const std::vector<GeneratedContent> &results, const CliOptions &cliOptions) {
  for (auto &result: results) {
    std::filesystem::path target;
    switch (result.mType) {
    case FileType::CppHeader:
    case FileType::CppSource:
    case FileType::CMakeFile:
      target = std::filesystem::path(cliOptions.mOutDir) / result.mName;
      break;
    case FileType::CSharpProject:
    case FileType::CSharpWrapper:
      target = std::filesystem::path(cliOptions.mCSharpRoot) / result.mName;
      break;
    }
    THROW_IF(target.empty(), "Could not figure out where to put {}", result.mName);
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
}

int Run(const CliOptions &cliOptions) {
  auto projectDefinition = ParseProjectDefinition(cliOptions);
  Translator translator{GetTranslatorSettings(cliOptions)};
  auto project = translator.Translate(projectDefinition);
  auto generatorSettings = GetGeneratorSettings(cliOptions);
  auto generator = CodeGenerator(generatorSettings);
  auto results = generator.Generate(project);
  WriteToFiles(results, cliOptions);
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
