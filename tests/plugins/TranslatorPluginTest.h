#include <gtest/gtest.h>
#include "core/Exception.h"
#include "generator/plugins/TranslatorPlugin.h"
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"
#include "../Helpers.h"

using namespace holgen;

class TranslatorPluginTest : public ::testing::Test {
protected:
  void SetUp() override {
    mParser = Parser();
  }

  void TearDown() override {
  }

  TranslatedProject Parse(std::string_view input) {
    Tokenizer tokenizer(input, Source);
    mParser.Parse(tokenizer);
    return TranslatedProject(mParser.GetProject());
  }

  template<typename... Args>
  void ExpectErrorMessage(const std::string &input,
                          std::function<void(TranslatedProject &)> run,
                          std::format_string<Args...> fmt, Args &&...args) {
    Reset();
    auto project = Parse(helpers::Trim(input));
    EXPECT_THROW({
                   try {
                     run(project);
                   } catch (Exception &exc) {
                     std::string actualError = exc.what();
                     actualError = actualError.substr(actualError.find(' ') + 1);
                     EXPECT_EQ(actualError, std::format(fmt, std::forward<Args>(args)...));
                     throw;
                   }
                 }, Exception);
  }

  Parser mParser;
  const char *Source = "TranslatorPluginTest";
private:
  void Reset() {
    mParser = {};
  }
};
