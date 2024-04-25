#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldVariantPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"

class ClassFieldVariantPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
    ClassFieldVariantPlugin(project, {}).Run();
  }
};

TEST_F(ClassFieldVariantPluginTest, VariantField) {
  auto project = Parse(R"R(
enum DataType {
  Type1;
  Type2;
}
struct TestData {
  @variant(enum=DataType, typeField=dataType)
  variant data;
}
@variant(enum=DataType, entry=Type1)
struct TestDataType1 {
  float f1;
  double f2;
}
@variant(enum=DataType, entry=Type2)
struct TestDataType2 {
  u64 f1;
  u64 f2;
  string f3;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->mStruct, nullptr);
  EXPECT_EQ(cls->mEnum, nullptr);

  ASSERT_NE(cls->GetField("mDataType"), nullptr);
  {
    auto field = ClassField{"mDataType", Type{"DataType"}};
    helpers::ExpectEqual(*cls->GetField("mDataType"), field);
  }

  ASSERT_NE(cls->GetField("mData"), nullptr);
  {
    auto field = ClassField{"mData", Type{"std::array"}};
    field.mType.mTemplateParameters.emplace_back("uint8_t");
    field.mType.mTemplateParameters.emplace_back("std::max(sizeof(TestDataType1), sizeof(TestDataType2))");
    field.mField = cls->mStruct->GetField("data");
    helpers::ExpectEqual(*cls->GetField("mData"), field);
  }
}
