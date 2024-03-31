#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/json/JsonPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"

class JsonPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project).Run();
    ClassFieldPlugin(project).Run();
    JsonConverterPlugin(project).Run();
    // Not a direct dependency, but needed to parse lua function names
    LuaFunctionPlugin(project).Run();
    JsonPlugin(project).Run();
  }
};

TEST_F(JsonPluginTest, StructParseJsonEmpty) {
  auto project = Parse("struct TestData {}");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseJson", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseJson", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
for(const auto& data: json.GetObject()) {
  const auto& name = data.name.GetString();
}
return true;
    )R");
  }
}

TEST_F(JsonPluginTest, StructParseJson) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned;
  double testFieldDouble;
  bool testFieldBool;

  @luaFunc
  func testFieldFunc(u32 a1) -> bool;
}
      )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseJson", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseJson", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
for(const auto& data: json.GetObject()) {
  const auto& name = data.name.GetString();
  if (0 == strcmp(name, "testFieldUnsigned")) {
    auto res = JsonHelper::Parse(mTestFieldUnsigned, data.value, converter);
    if (!res)
      return false;
  } else if (0 == strcmp(name, "testFieldDouble")) {
    auto res = JsonHelper::Parse(mTestFieldDouble, data.value, converter);
    if (!res)
      return false;
  } else if (0 == strcmp(name, "testFieldBool")) {
    auto res = JsonHelper::Parse(mTestFieldBool, data.value, converter);
    if (!res)
      return false;
  } else if (0 == strcmp(name, "testFieldFunc")) {
    auto res = JsonHelper::Parse(mFuncName_testFieldFunc, data.value, converter);
    if (!res)
      return false;
  }
}
return true;
    )R");
  }
}

TEST_F(JsonPluginTest, StructParseJsonConverter) {
  auto project = Parse(R"R(
struct TestData {
  @jsonConvert(from=string, using=testU32Converter)
  u32 testFieldUnsigned;
  @jsonConvert(from=bool, using=testStringConverter)
  string testFieldString;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseJson", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseJson", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
for(const auto& data: json.GetObject()) {
  const auto& name = data.name.GetString();
  if (0 == strcmp(name, "testFieldUnsigned")) {
    std::string temp;
    auto res = JsonHelper::Parse(temp, data.value, converter);
    if (!res)
      return false;
    mTestFieldUnsigned = converter.testU32Converter(temp);
  } else if (0 == strcmp(name, "testFieldString")) {
    bool temp;
    auto res = JsonHelper::Parse(temp, data.value, converter);
    if (!res)
      return false;
    mTestFieldString = std::move(converter.testStringConverter(temp));
  }
}
return true;
    )R");
  }
}

TEST_F(JsonPluginTest, EnumParseJson) {
  auto project = Parse("enum TestData {}");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseJson", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseJson", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter", Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
if (json.IsString()) {
  *this = TestData::FromString(std::string_view(json.GetString(), json.GetStringLength()));
} else if (json.IsInt64()) {
  *this = TestData(json.GetInt64());
} else {
  *this = TestData(TestData::Invalid);
  return false;
}
return true;
    )R");
  }
}
