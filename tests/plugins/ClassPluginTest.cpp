#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"

class ClassPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
  }
};

TEST_F(ClassPluginTest, Struct) {
  auto project = Parse(R"R(
struct TestData {
}
      )R");
  Run(project);
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
  })R",
      Run, "Duplicate class: TestData ({0}:1:1) and TestData ({0}:3:1)", Source);
}

TEST_F(ClassPluginTest, ReservedKeywords) {
  ExpectErrorMessage("struct struct {}", Run, "struct ({0}:1:1) is a reserved keyword", Source);
  ExpectErrorMessage("struct class {}", Run, "class ({0}:1:1) is a reserved keyword", Source);
  ExpectErrorMessage("struct namespace {}", Run, "namespace ({0}:1:1) is a reserved keyword",
                     Source);
  ExpectErrorMessage("struct template {}", Run, "template ({0}:1:1) is a reserved keyword", Source);
  ExpectErrorMessage("struct uint16_t {}", Run, "uint16_t ({0}:1:1) is a reserved keyword", Source);
}

TEST_F(ClassPluginTest, UnrecognizedMixin) {
  ExpectErrorMessage("struct A: B {}", Run, "A ({0}:1:1) uses unrecognized mixin B", Source);
  ExpectErrorMessage("struct A: B, C {} mixin B {}", Run, "A ({0}:1:1) uses unrecognized mixin C",
                     Source);
  ExpectErrorMessage("struct A: B {} mixin B: C {}", Run, "B ({0}:1:16) uses unrecognized mixin C",
                     Source);
}

TEST_F(ClassPluginTest, CircularMixin) {
  ExpectErrorMessage("struct A: A {}", Run,
                     "Circular or duplicate mixin usage detected in A ({0}:1:1) and A ({0}:1:1)",
                     Source);
  ExpectErrorMessage(
      R"R(
struct A: B, C {}
mixin B: C {}
mixin C: A {}
  )R",
      Run, "Circular or duplicate mixin usage detected in A ({0}:1:1) and C ({0}:3:1)", Source);
  ExpectErrorMessage(
      R"R(
struct A: B {}
mixin B: C {}
mixin C: A {}
  )R",
      Run, "Circular or duplicate mixin usage detected in A ({0}:1:1) and C ({0}:3:1)", Source);
  ExpectErrorMessage(
      R"R(
struct A: B {}
mixin B: C {}
mixin C: B {}
)R",
      Run, "Circular or duplicate mixin usage detected in A ({0}:1:1) and C ({0}:3:1)", Source);
}

TEST_F(ClassPluginTest, Enum) {
  auto project = Parse(R"R(
enum TestData {
}
  )R");
  Run(project);
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
                     Run, "Duplicate class: TestData ({0}:1:1) and TestData ({0}:3:1)", Source);
}
