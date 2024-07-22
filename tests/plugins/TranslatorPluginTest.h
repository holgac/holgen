#include <gtest/gtest.h>
#include "../Helpers.h"
#include "core/Exception.h"
#include "generator/plugins/TranslatorPlugin.h"
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"

using namespace holgen;

class TranslatorPluginTest : public ::testing::Test {
protected:
  ProjectDefinition mProjectDefinition;

  void SetUp() override {
    mProjectDefinition = {};
  }

  void TearDown() override {}

  TranslatedProject Parse(std::string_view input) {
    Tokenizer tokenizer(input, Source);
    Parser{mProjectDefinition, tokenizer}.Parse();
    return TranslatedProject(mProjectDefinition);
  }

  template <typename... Args>
  void ExpectErrorMessage(const std::string &input, std::function<void(TranslatedProject &)> run,
                          std::format_string<Args...> fmt, Args &&...args) {
    mProjectDefinition = {};
    EXPECT_THROW(
        {
          try {
            auto project = Parse(helpers::Trim(input));
            run(project);
          } catch (Exception &exc) {
            std::string actualError = exc.what();
            actualError = actualError.substr(actualError.find(' ') + 1);
            EXPECT_EQ(actualError, std::format(fmt, std::forward<Args>(args)...));
            throw;
          }
        },
        Exception);
  }

  const char *Source = "TranslatorPluginTest";
};
