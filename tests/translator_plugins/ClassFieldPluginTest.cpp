#include "TranslatorPluginTest.h"
#include "generator/translator_plugins/ClassPlugin.h"
#include "generator/translator_plugins/ClassFieldPlugin.h"
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
  helpers::ExpectTypeEqual(
      cls->GetField("mTestFieldUnsigned")->mType,
      Type{"uint32_t"});
  EXPECT_EQ(cls->GetField("mTestFieldUnsigned")->mDefaultValue, "42");

  ASSERT_NE(cls->GetField("mTestFieldDouble"), nullptr);
  helpers::ExpectTypeEqual(
      cls->GetField("mTestFieldDouble")->mType,
      Type{"double"});

  ASSERT_NE(cls->GetField("mTestFieldBool"), nullptr);
  helpers::ExpectTypeEqual(
      cls->GetField("mTestFieldBool")->mType,
      Type{"bool"});

  ASSERT_NE(cls->GetField("mTestFieldString"), nullptr);
  helpers::ExpectTypeEqual(
      cls->GetField("mTestFieldString")->mType,
      Type{"std::string"});
}

TEST_F(ClassFieldPluginTest, Containers) {
  auto project = Parse(R"R(
struct InnerData {
  u32 innerField;
}
struct TestData {
  vector<InnerData> testFieldVector;
  map<string, InnerData> testFieldMap;
}
      )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);;

  ASSERT_NE(cls->GetField("mTestFieldVector"), nullptr);
  auto vecType = Type{"std::vector"};
  vecType.mTemplateParameters.emplace_back("InnerData");
  helpers::ExpectTypeEqual(cls->GetField("mTestFieldVector")->mType, vecType);

  ASSERT_NE(cls->GetField("mTestFieldMap"), nullptr);
  auto mapType = Type{"std::map"};
  mapType.mTemplateParameters.emplace_back("std::string");
  mapType.mTemplateParameters.emplace_back("InnerData");
  helpers::ExpectTypeEqual(cls->GetField("mTestFieldMap")->mType, mapType);
}
