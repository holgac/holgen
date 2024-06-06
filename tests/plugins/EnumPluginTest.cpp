#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/enum/EnumPlugin.h"

class EnumPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    EnumPlugin(project, {}).Run();
  }
};

TEST_F(EnumPluginTest, EmptyEnum) {
  auto project = Parse(R"R(
enum TestEnum {
}
      )R");
  Run(project);
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mStruct, nullptr);
  EXPECT_NE(cls->mEnum, nullptr);
  EXPECT_EQ(project.mClasses.size(), 1);

  // TODO: find a way to test ctor overloads
  /*
  EXPECT_EQ(cls->mConstructors.size(), 1);
  auto ctor = ClassConstructor{};
  ctor.mExplicitness = Explicitness::Explicit;
  ctor.mArguments.emplace_back("value", Type{"UnderlyingType"}, "Invalid");
  ctor.mInitializerList.emplace_back("mValue", "value");
  helpers::ExpectEqual(cls->mConstructors.front(), ctor);
  helpers::ExpectEqual(*cls->GetField("mValue"), ClassField{"mValue", Type{"UnderlyingType"}});
  */
}

TEST_F(EnumPluginTest, Entries) {
  auto project = Parse(R"R(
enum TestEnum {
  Entry1;
  Entry2 = 3;
  Entry3 = 1;
}
  )R");
  Run(project);
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  std::vector<ClassEnumEntry> expectedEntries = {
      {"Entry1", "0"},
      {"Entry2", "3"},
      {"Entry3", "1"},
  };

  ASSERT_EQ(cls->mNestedEnums.size(), 1);
  auto &nestedEnum = cls->mNestedEnums.front();

  EXPECT_EQ(nestedEnum.mEntries.size(), expectedEntries.size());
  for (auto &expectedEntry: expectedEntries) {
    auto actualEntry = nestedEnum.GetEntry(expectedEntry.mName);
    ASSERT_NE(actualEntry, nullptr);
    helpers::ExpectEqual(*actualEntry, expectedEntry);
  }

  {
    ASSERT_NE(cls->GetField("Invalid"), nullptr);
    auto field = ClassField{"Invalid", Type{"UnderlyingType", PassByType::Value, Constness::Const},
                            Visibility::Public, Staticness::Static, "2"};
    field.mType.mConstexprness = Constexprness::Constexpr;
    helpers::ExpectEqual(*cls->GetField("Invalid"), field);
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
  Run(project);
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"FromString", Type{"TestEnum"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mArguments.emplace_back("str", Type{"std::string_view"});
  helpers::ExpectEqual(*cls->GetMethod("FromString", Constness::NotConst), method, R"R(
if (str == "Entry1") {
  return TestEnum(TestEnum::Entry1);
} else if (str == "Entry2") {
  return TestEnum(TestEnum::Entry2);
} else if (str == "Entry3") {
  return TestEnum(TestEnum::Entry3);
} else {
  HOLGEN_WARN("{} is not a valid TestEnum, returning invalid", str);
  return TestEnum{};
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
  Run(project);
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"ToString", Type{"char", PassByType::Pointer, Constness::Const}};
  ASSERT_NE(cls->GetMethod("ToString", Constness::Const), nullptr);
  helpers::ExpectEqual(*cls->GetMethod("ToString", Constness::Const), method, R"R(
switch (mValue) {
  case TestEnum::Entry1:
    return "Entry1";
  case TestEnum::Entry2:
    return "Entry2";
  case TestEnum::Entry3:
    return "Entry3";
  default:
    return "INVALID";
}
  )R");
}

TEST_F(EnumPluginTest, GetEntries) {
  auto project = Parse(R"R(
enum TestEnum {
  Entry1;
  Entry2 = 3;
  Entry3 = 2;
}
  )R");
  Run(project);
  EXPECT_EQ(project.mClasses.size(), 1);
  auto cls = project.GetClass("TestEnum");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"GetEntries", Type{"std::array"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mReturnType.mTemplateParameters.emplace_back("TestEnum::Entry");
  method.mReturnType.mTemplateParameters.emplace_back("3");
  ASSERT_NE(cls->GetMethod("GetEntries", Constness::NotConst), nullptr);
  helpers::ExpectEqual(*cls->GetMethod("GetEntries", Constness::NotConst), method, R"R(
return std::array<TestEnum::Entry, 3>{Entry1, Entry2, Entry3};
  )R");
}

TEST_F(EnumPluginTest, DuplicateEntry) {
  ExpectErrorMessage(
      R"R(
enum TestEnum {
  Entry1 = 5;
  Entry1 = 5;
}
  )R",
      Run,
      "Enum TestEnum ({0}:1:1) contains duplicate entries TestEnum.Entry1 ({0}:2:3) and "
      "TestEnum.Entry1 ({0}:3:3)",
      Source);
}
