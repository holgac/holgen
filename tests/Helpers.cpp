#include "Helpers.h"
#include <gtest/gtest.h>

namespace holgen::helpers {

  std::map<std::string, GeneratedContent> MapByName(const std::vector<GeneratedContent> &contents) {
    std::map<std::string, GeneratedContent> result;
    for (const auto &gc: contents) {
      auto[it, res] = result.emplace(gc.mName, gc);
      EXPECT_TRUE(res);
    }
    return result;
  }

  std::string_view Trim(const std::string &str) {
    auto startIdx = str.find_first_not_of(" \n");
    auto endIdx = str.find_last_not_of(" \n");
    return std::string_view(str.data() + startIdx, endIdx - startIdx);
  }

  void ExpectGeneratedContent(const GeneratedContent &actual, const GeneratedContent &expected) {
    EXPECT_EQ(actual.mType, expected.mType);
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(Trim(actual.mText), Trim(expected.mText));
  }

  void ExpectTypeEqual(const Type &expected, const Type &actual) {
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(actual.mType, expected.mType);
    EXPECT_EQ(actual.mConstness, expected.mConstness);
    EXPECT_EQ(actual.mConstexprness, expected.mConstexprness);
    EXPECT_EQ(actual.mTemplateParameters.size(), expected.mTemplateParameters.size());
    for(size_t i = 0; i < actual.mTemplateParameters.size(); ++i) {
      ExpectTypeEqual(expected.mTemplateParameters[i], actual.mTemplateParameters[i]);
    }
    EXPECT_EQ(actual.mFunctionalTemplateParameters.size(), expected.mFunctionalTemplateParameters.size());
    for(size_t i = 0; i < actual.mFunctionalTemplateParameters.size(); ++i) {
      ExpectTypeEqual(expected.mFunctionalTemplateParameters[i], actual.mFunctionalTemplateParameters[i]);
    }
  }
}
