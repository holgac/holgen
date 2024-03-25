#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "../Helpers.h"

class ClassFieldPluginTest : public TranslatorPluginTest {

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
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);;

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
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);;

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
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

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
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  ASSERT_NE(cls->GetField("mTestFieldRefId"), nullptr);
  {
    auto field = ClassField{"mTestFieldRefId", Type{"uint32_t"}};
    field.mField = cls->mStruct->GetField("testFieldRef");
    field.mDefaultValue = "-1";
    helpers::ExpectEqual(*cls->GetField("mTestFieldRefId"), field);
  }
}
