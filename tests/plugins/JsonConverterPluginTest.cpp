#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "../Helpers.h"

class JsonConverterPluginTest : public TranslatorPluginTest {
public:
  TranslatedProject RunPlugins(const char *str) {
    auto project = Parse(str);
    ClassPlugin(project).Run();
    ClassFieldPlugin(project).Run();
    JsonConverterPlugin(project).Run();
    return project;
  }
};

TEST_F(JsonConverterPluginTest, PrimitiveToComplex) {
  auto project = RunPlugins(R"R(
struct TestData {
  @jsonConvert(from=u32, using=u32ToString)
  string testFieldString;
}
  )R");
  auto cls = project.GetClass("Converter");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->GetField("u32ToString"), nullptr);
  {
    auto field = ClassField{"u32ToString", Type{"std::function"}, Visibility::Public};
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"std::string"});
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"uint32_t"});
    helpers::ExpectEqual(*cls->GetField("u32ToString"), field);
  }
}

TEST_F(JsonConverterPluginTest, CustomToCustom) {
  auto project = RunPlugins(R"R(
struct InnerStruct {}
struct InnerStruct2 {}
struct TestData {
  @jsonConvert(from=InnerStruct2, using=innerStruct2ToInnerStruct)
  InnerStruct testFieldString;
}
  )R");
  auto cls = project.GetClass("Converter");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->GetField("innerStruct2ToInnerStruct"), nullptr);
  {
    auto field = ClassField{"innerStruct2ToInnerStruct", Type{"std::function"}, Visibility::Public};
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"InnerStruct"});
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"InnerStruct2", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetField("innerStruct2ToInnerStruct"), field);
  }
}

TEST_F(JsonConverterPluginTest, ComplexToPrimitive) {
  auto project = RunPlugins(R"R(
struct TestData {
  @jsonConvert(from=string, using=stringToU32)
  u32 testFieldUnsigned;
}
  )R");
  auto cls = project.GetClass("Converter");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->GetField("stringToU32"), nullptr);
  {
    auto field = ClassField{"stringToU32", Type{"std::function"}, Visibility::Public};
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"uint32_t"});
    field.mType.mFunctionalTemplateParameters.emplace_back(
        Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetField("stringToU32"), field);
  }
}

TEST_F(JsonConverterPluginTest, ReusedConverter) {
  auto project = RunPlugins(R"R(
struct TestData {
  @jsonConvert(from=u32, using=u32ToString)
  string testFieldString;
  @jsonConvert(from=u32, using=u32ToString)
  string testFieldString2;
}
struct TestData2 {
  @jsonConvert(from=u32, using=u32ToString)
  string testFieldString;
  @jsonConvert(from=u32, using=u32ToString)
  string testFieldString2;
}
  )R");
  auto cls = project.GetClass("Converter");
  ASSERT_NE(cls, nullptr);
  ASSERT_EQ(cls->mFields.size(), 1);

  EXPECT_NE(cls->GetField("u32ToString"), nullptr);
  {
    auto field = ClassField{"u32ToString", Type{"std::function"}, Visibility::Public};
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"std::string"});
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"uint32_t"});
    helpers::ExpectEqual(*cls->GetField("u32ToString"), field);
  }
}
