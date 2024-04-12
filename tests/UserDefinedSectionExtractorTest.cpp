#include <gtest/gtest.h>
#include "generator/CodeBlock.h"
#include "tokenizer/Tokenizer.h"
#include "parser/Parser.h"
#include "generator/UserDefinedSectionExtractor.h"
#include "core/Exception.h"

using namespace holgen;

namespace {
}

class UserDefinedSectionExtractorTest : public ::testing::Test {
protected:
  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(UserDefinedSectionExtractorTest, Extract) {
  const char *contents = R"R(
some generated content
// HOLGEN_USER_DEFINED_BEGIN:EmptySection
// HOLGEN_USER_DEFINED_END:EmptySection
Some other content
// HOLGEN_USER_DEFINED_BEGIN:NonEmptySection
Content
Goes
Here
// HOLGEN_USER_DEFINED_END:NonEmptySection
even more content
)R";
  const char *expectedSectionContents = R"R(Content
Goes
Here
)R";
  UserDefinedSectionExtractor extractor;
  auto sections = extractor.Extract(contents);
  ASSERT_EQ(sections.size(), 2);
  EXPECT_TRUE(sections.contains("EmptySection"));
  EXPECT_EQ(sections.at("EmptySection"), "");
  EXPECT_TRUE(sections.contains("NonEmptySection"));
  EXPECT_EQ(sections.at("NonEmptySection"), expectedSectionContents);
}

TEST_F(UserDefinedSectionExtractorTest, CodeBlockToString) {
  CodeBlock codeBlock;
  codeBlock.Add("some generated content");
  codeBlock.UserDefined("EmptySection");
  codeBlock.Add("Some other content");
  codeBlock.UserDefined("NonEmptySection");
  codeBlock.Add("even more content");
  const char *emptyContents = R"R(some generated content
// HOLGEN_USER_DEFINED_BEGIN:EmptySection
// HOLGEN_USER_DEFINED_END:EmptySection
Some other content
// HOLGEN_USER_DEFINED_BEGIN:NonEmptySection
// HOLGEN_USER_DEFINED_END:NonEmptySection
even more content
)R";
  EXPECT_EQ(codeBlock.ToString(FileType::CppSource, {}), emptyContents);
  const char *populatedContents = R"R(some generated content
// HOLGEN_USER_DEFINED_BEGIN:EmptySection
// HOLGEN_USER_DEFINED_END:EmptySection
Some other content
// HOLGEN_USER_DEFINED_BEGIN:NonEmptySection
Content
Goes
Here
// HOLGEN_USER_DEFINED_END:NonEmptySection
even more content
)R";
  auto sections = UserDefinedSectionExtractor().Extract(populatedContents);
  EXPECT_EQ(codeBlock.ToString(FileType::CppSource, sections), populatedContents);
}
