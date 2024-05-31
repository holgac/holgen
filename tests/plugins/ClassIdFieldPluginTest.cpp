#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"
#include "generator/plugins/ClassPlugin.h"

class ClassIdFieldPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
  }
};

TEST_F(ClassIdFieldPluginTest, Primitives) {
  auto project = Parse(R"R(
struct TestData {
  @id
  u32 testFieldUnsigned;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetField("mTestFieldUnsigned"), nullptr);
  {
    auto field = ClassField{"mTestFieldUnsigned", Type{"uint32_t"}};
    field.mField = cls->mStruct->GetField("testFieldUnsigned");
    field.mDefaultValue = "-1";
    helpers::ExpectEqual(*cls->GetField("mTestFieldUnsigned"), field);
  }
  EXPECT_EQ(cls->GetField("mTestFieldUnsigned"), cls->GetIdField());
}

TEST_F(ClassIdFieldPluginTest, Mixin) {
  auto project = Parse(R"R(
mixin M {
  @id
  u32 testFieldUnsigned;
}
struct TestData : M {}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetField("mTestFieldUnsigned"), nullptr);
  {
    auto field = ClassField{"mTestFieldUnsigned", Type{"uint32_t"}};
    field.mField = project.mProject.GetStruct("M")->GetField("testFieldUnsigned");
    field.mDefaultValue = "-1";
    helpers::ExpectEqual(*cls->GetField("mTestFieldUnsigned"), field);
  }
  EXPECT_EQ(cls->GetField("mTestFieldUnsigned"), cls->GetIdField());
}

TEST_F(ClassIdFieldPluginTest, InvalidTypes) {
  ExpectErrorMessage("struct A {@id vector<s32> field;}", Run,
                     "A.field ({0}:1:15) uses an invalid type for an id: std::vector", Source);
  ExpectErrorMessage("struct A {@id userdata field;}", Run, "Unknown type userdata used by A.field ({0}:1:15)", Source);
}

TEST_F(ClassIdFieldPluginTest, MultipleIds) {
  ExpectErrorMessage(
      R"R(
struct A {
  @id
  s32 field1;
  @id
  s32 field2;
}
)R",
      Run, "A ({0}:1:1) has multiple id fields: A.field1 ({0}:3:3) and A.field2 ({0}:5:3)", Source);
}

TEST_F(ClassIdFieldPluginTest, MultipleIdsInMixins) {
  ExpectErrorMessage(
      R"R(
struct A : M1, M2 {}
mixin M1 {
  @id
  s32 field1;
}
mixin M2 {
  @id
  s32 field2;
}
      )R",
      Run, "A ({0}:1:1) has multiple id fields: A.field1 ({0}:4:3) and A.field2 ({0}:8:3)", Source);
}
