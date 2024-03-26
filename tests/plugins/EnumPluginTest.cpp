#include "TranslatorPluginTest.h"
#include "generator/plugins/EnumPlugin.h"
#include "generator/plugins/ClassPlugin.h"
#include "../Helpers.h"

class EnumPluginTest : public TranslatorPluginTest {

};

TEST_F(EnumPluginTest, EmptyEnum) {
  auto project = Parse(R"R(
enum TestEnum {
}
      )R");
  ClassPlugin(project).Run();
  EnumPlugin(project).Run();
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mStruct, nullptr);
  EXPECT_NE(cls->mEnum, nullptr);
  EXPECT_EQ(project.mClasses.size(), 1);

  EXPECT_EQ(cls->mConstructors.size(), 1);

  auto ctor = ClassConstructor{};
  ctor.mExplicitness = Explicitness::Explicit;
  ctor.mArguments.emplace_back("value", Type{"UnderlyingType"}, "Invalid");
  ctor.mInitializerList.emplace_back("mValue", "value");
  helpers::ExpectEqual(cls->mConstructors[0], ctor);
  helpers::ExpectEqual(*cls->GetField("mValue"), ClassField{"mValue", Type{"UnderlyingType"}});
}

TEST_F(EnumPluginTest, Entries) {
  auto project = Parse(R"R(
enum TestEnum {
  Entry1;
  Entry2 = 3;
  Entry3 = 1;
}
  )R");
  ClassPlugin(project).Run();
  EnumPlugin(project).Run();
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  std::vector<std::pair<std::string, std::string>> expectedEntries = {
      {"Entry1", "0"},
      {"Entry2", "3"},
      {"Entry3", "1"},
  };

  for (auto&[name, value]: expectedEntries) {
    ASSERT_NE(cls->GetField(name + "Value"), nullptr);
    auto field = ClassField{
        name + "Value",
        Type{"UnderlyingType", PassByType::Value, Constness::Const},
        Visibility::Public,
        Staticness::Static,
        value
    };
    field.mType.mConstexprness = Constexprness::Constexpr;
    helpers::ExpectEqual(*cls->GetField(name + "Value"), field);
  }

  {
    ASSERT_NE(cls->GetField("Invalid"), nullptr);
    auto field = ClassField{
        "Invalid",
        Type{"UnderlyingType", PassByType::Value, Constness::Const},
        Visibility::Public,
        Staticness::Static,
        "2"
    };
    field.mType.mConstexprness = Constexprness::Constexpr;
    helpers::ExpectEqual(*cls->GetField("Invalid"), field);
  }

  for (auto&[name, value]: expectedEntries) {
    ASSERT_NE(cls->GetField(name), nullptr);
    auto field = ClassField{
        name,
        Type{"TestEnum", PassByType::Value, Constness::Const},
        Visibility::Public,
        Staticness::Static
    };
    field.mDefaultConstructorArguments.push_back(value);
    helpers::ExpectEqual(*cls->GetField(name), field);
  }
}

TEST_F(EnumPluginTest, FromString) {
  auto project = Parse(R"R(
enum TestEnum {
  Entry1;
  Entry2 = 3;
  Entry3 = 2;
}
  )R");
  ClassPlugin(project).Run();
  EnumPlugin(project).Run();
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{
      "FromString", Type{"TestEnum"},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("str", Type{"std::string_view"});
  helpers::ExpectEqual(*cls->GetMethod("FromString", Constness::NotConst), method, R"R(
if (str == "Entry1") {
  return TestEnum(0);
} else if (str == "Entry2") {
  return TestEnum(3);
} else if (str == "Entry3") {
  return TestEnum(2);
} else {
  return TestEnum(TestEnum::Invalid);
}
  )R");
}

TEST_F(EnumPluginTest, ToString) {
  auto project = Parse(R"R(
enum TestEnum {
  Entry1;
  Entry2 = 3;
  Entry3 = 2;
}
  )R");
  ClassPlugin(project).Run();
  EnumPlugin(project).Run();
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"ToString", Type{"TestEnum"}};
  helpers::ExpectEqual(*cls->GetMethod("ToString", Constness::NotConst), method, R"R(
switch (mValue) {
  case 0: return "Entry1";
  case 3: return "Entry2";
  case 2: return "Entry3";
  default: return "Invalid";
}
  )R");
}
