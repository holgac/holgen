#include <iostream>
#include <filesystem>
#include <fstream>
#include "core/Exception.h"
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"
#include "generator/Translator.h"
#include "generator/CodeGenerator.h"

int run(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: " << std::endl
              << argv[0] << " [IN_DIRECTORY] [OUT_DIRECTORY] [NAMESPACE] [CMAKE_TARGET]" << std::endl;
    return -1;
  }
  holgen::Parser parser;
  for (auto &entry: std::filesystem::directory_iterator(std::filesystem::path(argv[1]))) {
    if (!std::filesystem::is_regular_file(entry)) {
      continue;
    }
    std::ifstream fin(entry.path(), std::ios_base::binary);
    fin.seekg(0, std::ios_base::end);
    std::string contents(fin.tellg(), 0);
    fin.seekg(0, std::ios_base::beg);
    fin.read(contents.data(), contents.size());
    holgen::Tokenizer tokenizer(contents);
    parser.Parse(tokenizer);
  }
  holgen::Translator translator;
  auto project = translator.Translate(parser.GetProject());
  auto generator = holgen::CodeGenerator({argv[3], argv[4]});
  auto results = generator.Generate(project);
  std::filesystem::path outDir(argv[2]);
  for (auto &result: results) {
    // TODO: Write only if modified to avoid recompilation
    std::ofstream fout(outDir / result.mName);
    fout.write(result.mText.data(), result.mText.size());
  }
  return 0;
}

int main(int argc, char **argv) {
  try {
    return run(argc, argv);
  } catch (holgen::Exception &exc) {
    std::cerr << exc.what() << std::endl;
    return -1;
  }
}
