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

  void ExpectEqual(const Type &expected, const Type &actual) {
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(actual.mType, expected.mType) << expected.mName << " pass by type does not match";
    EXPECT_EQ(actual.mConstness, expected.mConstness);
    EXPECT_EQ(actual.mConstexprness, expected.mConstexprness);
    EXPECT_EQ(actual.mTemplateParameters.size(), expected.mTemplateParameters.size());
    for (size_t i = 0; i < actual.mTemplateParameters.size(); ++i) {
      ExpectEqual(expected.mTemplateParameters[i], actual.mTemplateParameters[i]);
    }
    EXPECT_EQ(actual.mFunctionalTemplateParameters.size(), expected.mFunctionalTemplateParameters.size());
    for (size_t i = 0; i < actual.mFunctionalTemplateParameters.size(); ++i) {
      ExpectEqual(expected.mFunctionalTemplateParameters[i], actual.mFunctionalTemplateParameters[i]);
    }
  }

  void ExpectEqual(const ClassField &actual, const ClassField &expected) {
    helpers::ExpectEqual(expected.mType, actual.mType);
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(actual.mVisibility, expected.mVisibility);
    EXPECT_EQ(actual.mStaticness, expected.mStaticness);
    EXPECT_EQ(actual.mDefaultValue, expected.mDefaultValue) << " in field " << actual.mName;
    EXPECT_EQ(actual.mDefaultConstructorArguments.size(), expected.mDefaultConstructorArguments.size());

    for (size_t i = 0; i < actual.mDefaultConstructorArguments.size(); ++i) {
      EXPECT_EQ(expected.mDefaultConstructorArguments[i], actual.mDefaultConstructorArguments[i]);
    }
    EXPECT_EQ(actual.mField, expected.mField) << " in field " << actual.mName;
  }

  void ExpectEqual(const ClassMethodBase &actual, const ClassMethodBase &expected,
                   const std::optional<std::string> &expectedBody) {
    EXPECT_EQ(actual.mVisibility, expected.mVisibility);
    if (expectedBody)
      EXPECT_EQ(Trim(actual.mBody.ToString()), Trim(*expectedBody));
    else
      EXPECT_EQ(actual.mBody.ToString(), expected.mBody.ToString());
    ASSERT_EQ(actual.mArguments.size(), expected.mArguments.size());
    for (size_t i = 0; i < actual.mArguments.size(); ++i) {
      ExpectEqual(actual.mArguments[i], expected.mArguments[i]);
    }
    ASSERT_EQ(actual.mTemplateParameters.size(), expected.mTemplateParameters.size());
    for (size_t i = 0; i < actual.mTemplateParameters.size(); ++i) {
      ExpectEqual(actual.mTemplateParameters[i], expected.mTemplateParameters[i]);
    }
  }

  void ExpectEqual(const ClassMethod &actual, const ClassMethod &expected,
                   const std::optional<std::string> &expectedBody) {
    ExpectEqual((ClassMethodBase &) actual, (ClassMethodBase &) expected, expectedBody);
    EXPECT_EQ(actual.mName, expected.mName);
    ExpectEqual(actual.mReturnType, expected.mReturnType);
    EXPECT_EQ(actual.mStaticness, expected.mStaticness);
    EXPECT_EQ(actual.mConstness, expected.mConstness) << " in method " << actual.mName;
    EXPECT_EQ(actual.mUserDefined, expected.mUserDefined);
    EXPECT_EQ(actual.mExposeToLua, expected.mExposeToLua);
  }

  void ExpectEqual(const ClassMethodArgument &actual, const ClassMethodArgument &expected) {
    ExpectEqual(actual.mType, expected.mType);
    EXPECT_EQ(actual.mName, expected.mName);
    EXPECT_EQ(actual.mDefaultValue, expected.mDefaultValue);
  }

  void ExpectEqual(const ClassConstructor &actual, const ClassConstructor &expected,
                   const std::optional<std::string> &expectedBody) {
    ExpectEqual((ClassMethodBase &) actual, (ClassMethodBase &) expected, expectedBody);
    EXPECT_EQ(actual.mExplicitness, expected.mExplicitness);
    for (size_t i = 0; i < actual.mInitializerList.size(); ++i) {
      ExpectEqual(actual.mInitializerList[i], expected.mInitializerList[i]);
    }
  }

  void ExpectEqual(const ClassConstructorInitializer &actual, const ClassConstructorInitializer &expected) {
    EXPECT_EQ(actual.mDestination, expected.mDestination);
    EXPECT_EQ(actual.mValue, expected.mValue);
  }

  void ExpectEqual(const TemplateParameter &actual, const TemplateParameter &expected) {
    EXPECT_EQ(actual.mType, expected.mType);
    EXPECT_EQ(actual.mName, expected.mName);
  }
}
