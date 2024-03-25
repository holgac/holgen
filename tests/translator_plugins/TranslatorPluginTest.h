#include <gtest/gtest.h>
#include "generator/translator_plugins/TranslatorPlugin.h"
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"

using namespace holgen;

class TranslatorPluginTest : public ::testing::Test {
protected:
  void SetUp() override {
    mParser = Parser();
  }

  void TearDown() override {
  }

  TranslatedProject Parse(const std::string &input) {
    Tokenizer tokenizer(input);
    mParser.Parse(tokenizer);
    return TranslatedProject(mParser.GetProject());
  }
  Parser mParser;
};
