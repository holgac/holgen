#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"

class ClassPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project).Run();
  }
};

TEST_F(ClassPluginTest, Struct) {
  auto project = Parse(R"R(
struct TestData {
}
      )R");
  ClassPlugin(project).Run();
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);
  EXPECT_EQ(project.mClasses.size(), 1);
}

TEST_F(ClassPluginTest, DuplicateStruct) {
  ExpectErrorMessage(
      R"R(
struct TestData {
}
struct TestData {
  })R", Run,
      "Duplicate class: TestData ({0}:1:1) and TestData ({0}:3:1)", Source);
}

TEST_F(ClassPluginTest, ReservedKeywords) {
  ExpectErrorMessage("struct struct {}", Run, "struct ({0}:1:1) is a reserved keyword", Source);
  Reset();
  ExpectErrorMessage("struct class {}", Run, "class ({0}:1:1) is a reserved keyword", Source);
  Reset();
  ExpectErrorMessage("struct namespace {}", Run, "namespace ({0}:1:1) is a reserved keyword", Source);
  Reset();
  ExpectErrorMessage("struct template {}", Run, "template ({0}:1:1) is a reserved keyword", Source);
}


TEST_F(ClassPluginTest, Enum) {
  auto project = Parse(R"R(
enum TestData {
}
  )R");
  ClassPlugin(project).Run();
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mStruct, nullptr);
  EXPECT_NE(cls->mEnum, nullptr);
  EXPECT_EQ(project.mClasses.size(), 1);
}

TEST_F(ClassPluginTest, DuplicateEnum) {
  ExpectErrorMessage(R"R(
enum TestData {
}
enum TestData {
})R",
                     Run,
                     "Duplicate class: TestData ({0}:1:1) and TestData ({0}:3:1)", Source);
}