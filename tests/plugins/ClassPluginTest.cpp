#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"

class ClassPluginTest : public TranslatorPluginTest {

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
