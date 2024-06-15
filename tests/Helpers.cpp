#include "Helpers.h"
#include <gtest/gtest.h>
#include "generator/FileType.h"

namespace holgen::helpers {

std::map<std::string, GeneratedContent> MapByName(const std::vector<GeneratedContent> &contents) {
  std::map<std::string, GeneratedContent> result;
  for (const auto &gc: contents) {
    auto [it, res] = result.emplace(gc.mName, gc);
    EXPECT_TRUE(res);
  }
  return result;
}

std::string_view Trim(const std::string_view &str) {
  auto startIdx = str.find_first_not_of(" \n");
  auto endIdx = str.find_last_not_of(" \n") + 1;
  return std::string_view(str.data() + startIdx, endIdx - startIdx);
}

void ExpectGeneratedContent(const GeneratedContent &actual, FileType fileType,
                            const std::string &name, const std::string &contents) {
  EXPECT_EQ(actual.mType, fileType);
  EXPECT_EQ(actual.mName, name);
  EXPECT_EQ(Trim(actual.mBody.ToString(fileType, {})), Trim(contents));
}

void ExpectEqual(const Type &expected, const Type &actual) {
  EXPECT_EQ(actual.mName, expected.mName);
  EXPECT_EQ(actual.mType, expected.mType) << " in type " << expected.mName;
  EXPECT_EQ(actual.mConstness, expected.mConstness) << " in type " << expected.mName;
  EXPECT_EQ(actual.mConstexprness, expected.mConstexprness);
  EXPECT_EQ(actual.mTemplateParameters.size(), expected.mTemplateParameters.size());
  for (size_t i = 0; i < actual.mTemplateParameters.size(); ++i) {
    ExpectEqual(expected.mTemplateParameters[i], actual.mTemplateParameters[i]);
  }
  EXPECT_EQ(actual.mFunctionalTemplateParameters.size(),
            expected.mFunctionalTemplateParameters.size());
  for (size_t i = 0; i < actual.mFunctionalTemplateParameters.size(); ++i) {
    ExpectEqual(expected.mFunctionalTemplateParameters[i], actual.mFunctionalTemplateParameters[i]);
  }
}

void ExpectEqual(const TypeDefinition &expected, const TypeDefinition &actual) {
  EXPECT_EQ(expected.mName, actual.mName);
  EXPECT_EQ(expected.mArraySize, actual.mArraySize);
  ASSERT_EQ(expected.mTemplateParameters.size(), actual.mTemplateParameters.size());
  for (size_t i = 0; i < expected.mTemplateParameters.size(); ++i) {
    ExpectEqual(expected.mTemplateParameters[i], actual.mTemplateParameters[i]);
  }
}

void ExpectEqual(const FunctionReturnTypeDefinition &expected,
                 const FunctionReturnTypeDefinition &actual) {
  ExpectEqual(expected.mType, actual.mType);
}

void ExpectEqual(const ClassField &actual, const ClassField &expected) {
  ExpectEqual(expected.mType, actual.mType);
  EXPECT_EQ(actual.mName, expected.mName);
  EXPECT_EQ(actual.mVisibility, expected.mVisibility) << " in field " << actual.mName;
  EXPECT_EQ(actual.mStaticness, expected.mStaticness);
  EXPECT_EQ(actual.mDefaultValue, expected.mDefaultValue) << " in field " << actual.mName;
  EXPECT_EQ(actual.mDefaultConstructorArguments.size(),
            expected.mDefaultConstructorArguments.size());

  for (auto itExpected = expected.mDefaultConstructorArguments.begin(),
            itActual = actual.mDefaultConstructorArguments.begin();
       itExpected != expected.mDefaultConstructorArguments.end(); ++itExpected, ++itActual) {
    EXPECT_EQ(*itActual, *itExpected);
  }
  EXPECT_EQ(actual.mField, expected.mField) << " in field " << actual.mName;
}

void ExpectEqual(const ClassMethodBase &actual, const ClassMethodBase &expected,
                 const std::optional<std::string> &expectedBody, const std::string &name) {
  EXPECT_EQ(actual.mVisibility, expected.mVisibility) << " in method " << name;
  if (expectedBody)
    EXPECT_EQ(Trim(actual.mBody.ToString(FileType::CppSource, {})), Trim(*expectedBody));
  else
    EXPECT_EQ(actual.mBody.ToString(FileType::CppSource, {}),
              expected.mBody.ToString(FileType::CppSource, {}));
  ASSERT_EQ(actual.mArguments.size(), expected.mArguments.size());
  for (auto itExpected = expected.mArguments.begin(), itActual = actual.mArguments.begin();
       itExpected != expected.mArguments.end(); ++itExpected, ++itActual) {
    ExpectEqual(*itActual, *itExpected);
  }
  ASSERT_EQ(actual.mTemplateParameters.size(), expected.mTemplateParameters.size());
  for (auto itExpected = expected.mTemplateParameters.begin(),
            itActual = actual.mTemplateParameters.begin();
       itExpected != expected.mTemplateParameters.end(); ++itExpected, ++itActual) {
    ExpectEqual(*itActual, *itExpected);
  }
}

void ExpectEqual(const ClassMethod &actual, const ClassMethod &expected,
                 const std::optional<std::string> &expectedBody) {
  ExpectEqual((ClassMethodBase &)actual, (ClassMethodBase &)expected, expectedBody, actual.mName);
  EXPECT_EQ(actual.mName, expected.mName);
  ExpectEqual(actual.mReturnType, expected.mReturnType);
  EXPECT_EQ(actual.mStaticness, expected.mStaticness);
  EXPECT_EQ(actual.mConstness, expected.mConstness) << " in method " << actual.mName;
  EXPECT_EQ(actual.mUserDefined, expected.mUserDefined);
  EXPECT_EQ(actual.mExposeToLua, expected.mExposeToLua);
  EXPECT_EQ(actual.mFunction, expected.mFunction);
}

void ExpectEqual(const ClassMethodArgument &actual, const ClassMethodArgument &expected) {
  ExpectEqual(actual.mType, expected.mType);
  EXPECT_EQ(actual.mName, expected.mName);
  EXPECT_EQ(actual.mDefaultValue, expected.mDefaultValue);
}

void ExpectEqual(const ClassDestructor &actual, const ClassDestructor &expected,
                 const std::optional<std::string> &expectedBody) {
  ExpectEqual((ClassMethodBase &)actual, (ClassMethodBase &)expected, expectedBody, "destructor");
}

void ExpectEqual(const ClassConstructor &actual, const ClassConstructor &expected,
                 const std::optional<std::string> &expectedBody) {
  ExpectEqual((ClassMethodBase &)actual, (ClassMethodBase &)expected, expectedBody, "constructor");
  EXPECT_EQ(actual.mExplicitness, expected.mExplicitness);
  for (auto itExpected = expected.mInitializerList.begin(),
            itActual = actual.mInitializerList.begin();
       itExpected != expected.mInitializerList.end(); ++itExpected, ++itActual) {
    ExpectEqual(*itActual, *itExpected);
  }
}

void ExpectEqual(const ClassConstructorInitializer &actual,
                 const ClassConstructorInitializer &expected) {
  EXPECT_EQ(actual.mDestination, expected.mDestination);
  EXPECT_EQ(actual.mValue, expected.mValue);
}

void ExpectEqual(const TemplateParameter &actual, const TemplateParameter &expected) {
  EXPECT_EQ(actual.mType, expected.mType);
  EXPECT_EQ(actual.mName, expected.mName);
}

void ExpectEqual(const ClassEnumEntry &actual, const ClassEnumEntry &expected) {
  EXPECT_EQ(actual.mName, expected.mName);
  EXPECT_EQ(actual.mValue, expected.mValue);
}
} // namespace holgen::helpers
