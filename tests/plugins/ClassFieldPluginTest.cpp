#include "TranslatorPluginTest.h"
#include "generator/plugins/cpp/ClassFieldPlugin.h"
#include "generator/plugins/cpp/ClassIdFieldPlugin.h"
#include "generator/plugins/cpp/ClassPlugin.h"

class ClassFieldPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
  }
};

TEST_F(ClassFieldPluginTest, Primitives) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned = 42;
  double testFieldDouble;
  bool testFieldBool;
  string testFieldString;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);

  ASSERT_NE(cls->GetField("mTestFieldUnsigned"), nullptr);
  {
    auto field = ClassField{"mTestFieldUnsigned", Type{"uint32_t"}};
    field.mField = cls->mStruct->GetField("testFieldUnsigned");
    field.mDefaultValue = "42";
    helpers::ExpectEqual(*cls->GetField("mTestFieldUnsigned"), field);
  }

  ASSERT_NE(cls->GetField("mTestFieldDouble"), nullptr);
  {
    auto field = ClassField{"mTestFieldDouble", Type{"double"}};
    field.mField = cls->mStruct->GetField("testFieldDouble");
    helpers::ExpectEqual(*cls->GetField("mTestFieldDouble"), field);
  }

  ASSERT_NE(cls->GetField("mTestFieldBool"), nullptr);
  {
    auto field = ClassField{"mTestFieldBool", Type{"bool"}};
    field.mField = cls->mStruct->GetField("testFieldBool");
    helpers::ExpectEqual(*cls->GetField("mTestFieldBool"), field);
  }

  ASSERT_NE(cls->GetField("mTestFieldString"), nullptr);
  {
    auto field = ClassField{"mTestFieldString", Type{"std::string"}};
    field.mField = cls->mStruct->GetField("testFieldString");
    helpers::ExpectEqual(*cls->GetField("mTestFieldString"), field);
  }
}

TEST_F(ClassFieldPluginTest, Mixins) {
  auto project = Parse(R"R(
mixin TestMixin1 {
  u32 testFieldUnsigned = 42;
}
mixin TestMixin2 : TestMixin1 {
  double testFieldDouble;
}
mixin TestMixin3 {
  bool testFieldBool;
}
struct TestData : TestMixin2, TestMixin3 {
  string testFieldString;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);

  ASSERT_NE(cls->GetField("mTestFieldUnsigned"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldDouble"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldBool"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldString"), nullptr);
  ASSERT_EQ(project.GetClass("TestMixin1"), nullptr);
  ASSERT_EQ(project.GetClass("TestMixin2"), nullptr);
  ASSERT_EQ(project.GetClass("TestMixin3"), nullptr);
}

TEST_F(ClassFieldPluginTest, Containers) {
  auto project = Parse(R"R(
struct InnerData {
  u32 innerField;
}
struct TestData {
  vector<InnerData> testFieldVector;
  map<string, vector<InnerData>> testFieldMap;
}
      )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);

  ASSERT_NE(cls->GetField("mTestFieldVector"), nullptr);
  {
    auto field = ClassField{"mTestFieldVector", Type{"std::vector"}};
    field.mType.mTemplateParameters.emplace_back("InnerData");
    field.mField = cls->mStruct->GetField("testFieldVector");
    helpers::ExpectEqual(*cls->GetField("mTestFieldVector"), field);
  }

  ASSERT_NE(cls->GetField("mTestFieldMap"), nullptr);
  {
    auto field = ClassField{"mTestFieldMap", Type{"std::map"}};
    field.mType.mTemplateParameters.emplace_back("std::string");
    field.mType.mTemplateParameters.emplace_back("std::vector");
    field.mType.mTemplateParameters.back().mTemplateParameters.emplace_back("InnerData");
    field.mField = cls->mStruct->GetField("testFieldMap");
    helpers::ExpectEqual(*cls->GetField("mTestFieldMap"), field);
  }
}

TEST_F(ClassFieldPluginTest, RefNoId) {
  auto project = Parse(R"R(
struct InnerData {
  u32 innerField;
}
struct TestData {
  Ref<InnerData> testFieldRef;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  EXPECT_EQ(cls->GetField("mTestFieldRefId"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldRef"), nullptr);
  {
    auto field = ClassField{"mTestFieldRef", Type{"InnerData", PassByType::Pointer}};
    field.mField = cls->mStruct->GetField("testFieldRef");
    field.mDefaultValue = "nullptr";
    helpers::ExpectEqual(*cls->GetField("mTestFieldRef"), field);
  }
}

TEST_F(ClassFieldPluginTest, RefWithId) {
  auto project = Parse(R"R(
struct InnerData {
  @id
  u32 innerField;
}
struct TestData {
  Ref<InnerData> testFieldRef;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  EXPECT_EQ(cls->GetField("mTestFieldRef"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldRefId"), nullptr);
  {
    auto field = ClassField{"mTestFieldRefId", Type{"uint32_t"}};
    field.mField = cls->mStruct->GetField("testFieldRef");
    field.mDefaultValue = "-1";
    helpers::ExpectEqual(*cls->GetField("mTestFieldRefId"), field);
  }
}

TEST_F(ClassFieldPluginTest, RefWithIdInMixin) {
  auto project = Parse(R"R(
mixin InnerMixin {
  @id
  u32 innerField;
}
struct InnerData: InnerMixin {
}
struct TestData {
  Ref<InnerData> testFieldRef;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  EXPECT_EQ(cls->GetField("mTestFieldRef"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldRefId"), nullptr);
  {
    auto field = ClassField{"mTestFieldRefId", Type{"uint32_t"}};
    field.mField = cls->mStruct->GetField("testFieldRef");
    field.mDefaultValue = "-1";
    helpers::ExpectEqual(*cls->GetField("mTestFieldRefId"), field);
  }
}

TEST_F(ClassFieldPluginTest, UserData) {
  auto project = Parse(R"R(
struct TestData {
  userdata testFieldUserData;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  ASSERT_NE(cls->GetField("mTestFieldUserData"), nullptr);
  {
    auto field = ClassField{"mTestFieldUserData", Type{"void", PassByType::Pointer}};
    field.mField = cls->mStruct->GetField("testFieldUserData");
    field.mDefaultValue = "nullptr";
    helpers::ExpectEqual(*cls->GetField("mTestFieldUserData"), field);
  }
}

TEST_F(ClassFieldPluginTest, ManagedRefWithId) {
  auto project = Parse(R"R(
struct DM { @container(elemName=innerDatum) vector<InnerData> innerData; }
@managed(by=DM, field=innerData)
struct InnerData {
  @id
  u32 innerField;
}
struct TestData {
  Ref<InnerData> testFieldRef;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  EXPECT_EQ(cls->GetField("mTestFieldRef"), nullptr);
  ASSERT_NE(cls->GetField("mTestFieldRefId"), nullptr);
  {
    auto field = ClassField{"mTestFieldRefId", Type{"uint32_t"}};
    field.mField = cls->mStruct->GetField("testFieldRef");
    field.mDefaultValue = "-1";
    helpers::ExpectEqual(*cls->GetField("mTestFieldRefId"), field);
  }
}

TEST_F(ClassFieldPluginTest, Arrays) {
  auto project = Parse(R"R(
enum TestEnum {
  Entry0;
  Entry1;
}
struct TestData {
  u32[1] testFieldArray1;
  u8[16] testFieldArray16;
  float[TestEnum] testFieldArrayEnum;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);

  ASSERT_NE(cls->GetField("mTestFieldArray1"), nullptr);
  {
    auto field = ClassField{"mTestFieldArray1", Type{"std::array"}};
    field.mField = cls->mStruct->GetField("testFieldArray1");
    field.mType.mTemplateParameters.emplace_back("uint32_t");
    field.mType.mTemplateParameters.emplace_back("1");
    helpers::ExpectEqual(*cls->GetField("mTestFieldArray1"), field);
  }

  ASSERT_NE(cls->GetField("mTestFieldArray16"), nullptr);
  {
    auto field = ClassField{"mTestFieldArray16", Type{"std::array"}};
    field.mField = cls->mStruct->GetField("testFieldArray16");
    field.mType.mTemplateParameters.emplace_back("uint8_t");
    field.mType.mTemplateParameters.emplace_back("16");
    helpers::ExpectEqual(*cls->GetField("mTestFieldArray16"), field);
  }

  ASSERT_NE(cls->GetField("mTestFieldArrayEnum"), nullptr);
  {
    auto field = ClassField{"mTestFieldArrayEnum", Type{"std::array"}};
    field.mField = cls->mStruct->GetField("testFieldArrayEnum");
    field.mType.mTemplateParameters.emplace_back("float");
    field.mType.mTemplateParameters.emplace_back("2");
    helpers::ExpectEqual(*cls->GetField("mTestFieldArrayEnum"), field);
  }
}

TEST_F(ClassFieldPluginTest, DuplicateField) {
  ExpectErrorMessage(R"R(
struct A {
  u32 field;
  u32 field;
})R",
                     Run, "Duplicate field: A.field ({0}:2:3) and A.field ({0}:3:3)", Source);
}

TEST_F(ClassFieldPluginTest, Vector) {
  ExpectErrorMessage(
      "struct A {u32<u32> field;}", Run,
      "Primitive type uint32_t used by A.field ({0}:1:11) cannot have template parameters", Source);
  ExpectErrorMessage("struct A {vector<u32, u32> field;}", Run,
                     "Container type std::vector used by A.field ({0}:1:11) should have a single "
                     "template parameter",
                     Source);
}

TEST_F(ClassFieldPluginTest, Set) {
  ExpectErrorMessage(
      "struct A {set<u32, u64, u32> field;}", Run,
      "Set type std::set used by A.field ({0}:1:11) should have a single template parameter",
      Source);
  ExpectErrorMessage("struct A {set<vector<u32>> field;}", Run,
                     "Set type std::set used by A.field ({0}:1:11) should have a keyable template "
                     "parameter, found std::vector",
                     Source);
  ExpectErrorMessage("struct A {set<B> field;} struct B {}", Run,
                     "Set type std::set used by A.field ({0}:1:11) should have a keyable template "
                     "parameter, found B",
                     Source);
}

TEST_F(ClassFieldPluginTest, Map) {
  ExpectErrorMessage("struct A {map<string, set<vector<u32>>> field;}", Run,
                     "Set type std::set used by A.field ({0}:1:11) should have a keyable template "
                     "parameter, found std::vector",
                     Source);
  ExpectErrorMessage(
      "struct A {map field;}", Run,
      "Map type std::map used by A.field ({0}:1:11) should have two template parameters", Source);
  ExpectErrorMessage(
      "struct A {map<u32> field;}", Run,
      "Map type std::map used by A.field ({0}:1:11) should have two template parameters", Source);
  ExpectErrorMessage(
      "struct A {map<u32, u64, u32> field;}", Run,
      "Map type std::map used by A.field ({0}:1:11) should have two template parameters", Source);
  ExpectErrorMessage("struct A {map<u32, vector<u32, u32>> field;}", Run,
                     "Container type std::vector used by A.field ({0}:1:11) should have a single "
                     "template parameter",
                     Source);
  ExpectErrorMessage("struct A {map<vector<u32>, u32> field;}", Run,
                     "Map key std::vector used by A.field ({0}:1:11) is not a valid key", Source);
}

TEST_F(ClassFieldPluginTest, InvalidType) {
  ExpectErrorMessage("struct A {u12 field;}", Run, "Unknown type u12 used by A.field ({0}:1:11)",
                     Source);
  ExpectErrorMessage("struct A {vector<B> field;}", Run,
                     "Unknown type B used by A.field ({0}:1:11)", Source);
  ExpectErrorMessage("struct A {map<u32, B> field;}", Run,
                     "Unknown type B used by A.field ({0}:1:11)", Source);
  ExpectErrorMessage("struct A {void field;}", Run, "Invalid void usage in A.field ({0}:1:11)",
                     Source);
  // TODO: test internal types like JsonHelper
}

TEST_F(ClassFieldPluginTest, RefType) {
  ExpectErrorMessage("struct A {Ref field;}", Run,
                     "Ref field A.field ({0}:1:11) should have a single template parameter",
                     Source);
  ExpectErrorMessage(
      "struct A {Ref<u32> field;}", Run,
      "Ref field A.field ({0}:1:11) references u32 which is not a struct defined in this project",
      Source);
}
