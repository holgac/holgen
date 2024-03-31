#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"

class JsonConverterPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project).Run();
    ClassFieldPlugin(project).Run();
    JsonConverterPlugin(project).Run();
  }
};

TEST_F(JsonConverterPluginTest, PrimitiveToComplex) {
  auto project = Parse(R"R(
struct TestData {
  @jsonConvert(from=u32, using=u32ToString)
  string testFieldString;
}
  )R");
  Run(project);
  auto cls = project.GetClass("Converter");
  ASSERT_NE(cls, nullptr);
  ASSERT_NE(cls->GetField("u32ToString"), nullptr);
  {
    auto field = ClassField{"u32ToString", Type{"std::function"}, Visibility::Public};
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"std::string"});
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"uint32_t"});
    helpers::ExpectEqual(*cls->GetField("u32ToString"), field);
  }
}

TEST_F(JsonConverterPluginTest, CustomToCustom) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct InnerStruct2 {}
struct TestData {
  @jsonConvert(from=InnerStruct2, using=innerStruct2ToInnerStruct)
  InnerStruct testFieldString;
}
  )R");
  Run(project);
  auto cls = project.GetClass("Converter");
  ASSERT_NE(cls, nullptr);
  EXPECT_NE(cls->GetField("innerStruct2ToInnerStruct"), nullptr);
  {
    auto field = ClassField{"innerStruct2ToInnerStruct", Type{"std::function"}, Visibility::Public};
    field.mType.mFunctionalTemplateParameters.emplace_back(Type{"InnerStruct"});
    field.mType.mFunctionalTemplateParameters.emplace_back(
        Type{"InnerStruct2", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetField("innerStruct2ToInnerStruct"), field);
  }
}

TEST_F(JsonConverterPluginTest, ComplexToPrimitive) {
  auto project = Parse(R"R(
struct TestData {
  @jsonConvert(from=string, using=stringToU32)
  u32 testFieldUnsigned;
}
  )R");
  Run(project);
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
  auto project = Parse(R"R(
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
  Run(project);
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

TEST_F(JsonConverterPluginTest, InconsistentSourceTypeInConverter) {
  ExpectErrorMessage(
      R"R(
struct A {
  @jsonConvert(from=string, using=testConverter)
  u32 unsignedField;
}
struct B {
  @jsonConvert(from=s32, using=testConverter)
  u32 unsignedField;
}
  )R", Run,
      "Json converter testConverter is used by A.unsignedField ({0}:3:3) and B.unsignedField ({0}:7:3) with different source types: std::string and int32_t",
      Source);
}

TEST_F(JsonConverterPluginTest, InconsistentTargetTypeInConverter) {
  ExpectErrorMessage(
      R"R(
struct A {
  @jsonConvert(from=string, using=testConverter)
  u32 unsignedField;
}
struct B {
  @jsonConvert(from=string, using=testConverter)
  bool boolField;
}
  )R", Run,
      "Json converter testConverter is used by A.unsignedField ({0}:3:3) and B.boolField ({0}:7:3) with different target types: uint32_t and bool",
      Source);
}

TEST_F(JsonConverterPluginTest, NonExistentConverterInIndex) {
  ExpectErrorMessage(
      R"R(
struct Person {
  @id
  u32 id;
}
struct Country {
  @index(on=uuid, forConverter=personNameToId)
  @container(elemName=person)
  vector<Person> people;
}
      )R", Run,
      "index ({0}:6:3) of Country.people ({0}:8:3) references non-existent converter personNameToId",
      Source);
}

TEST_F(JsonConverterPluginTest, InconsistentSourceTypeInIndex) {
  ExpectErrorMessage(
      R"R(
struct Person {
  @id
  u32 id;
  string name;
  u64 uuid;
  @jsonConvert(from=string, using=personNameToId)
  u32 partnerId;
}
struct Country {
  @index(on=uuid, forConverter=personNameToId)
  @container(elemName=person)
  vector<Person> people;
}
  )R", Run,
      "index ({0}:10:3) of Country.people ({0}:12:3) references converter personNameToId with different source type than Person.partnerId ({0}:7:3): uint64_t and std::string",
      Source);
}

TEST_F(JsonConverterPluginTest, InconsistentTargetTypeInIndex) {
  ExpectErrorMessage(
      R"R(
struct Person {
  @id
  u32 id;
  string name;
  u64 uuid;
  @jsonConvert(from=string, using=personNameToId)
  float gender;
}
struct Country {
  @index(on=name, forConverter=personNameToId)
  @container(elemName=person)
  vector<Person> people;
}
  )R", Run,
      "index ({0}:10:3) of Country.people ({0}:12:3) references converter personNameToId with different target type than Person.gender ({0}:7:3): uint32_t and float",
      Source);
}
