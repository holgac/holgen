#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"
#include "generator/plugins/json/JsonPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"

class JsonPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
    JsonConverterPlugin(project, {}).Run();
    // Not a direct dependency, but needed to parse lua function names
    LuaFunctionPlugin(project, {}).Run();
    JsonPlugin(project, {}).Run();
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
return true;
    )R");
  }
}

TEST_F(JsonPluginTest, StructParseJsonSingleElem) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned;
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
if (json.IsObject()) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp("testFieldUnsigned", name)) {
      auto res = JsonHelper::Parse(mTestFieldUnsigned, data.value, converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldUnsigned field");
    } else {
      HOLGEN_WARN("Unexpected entry in json when parsing TestData: {}", name);
    }
  }
} else {
  auto res = JsonHelper::Parse(mTestFieldUnsigned, json, converter);
  HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldUnsigned field");
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
HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestData");
for(const auto& data: json.GetObject()) {
  const auto& name = data.name.GetString();
  if (0 == strcmp("testFieldUnsigned", name)) {
    auto res = JsonHelper::Parse(mTestFieldUnsigned, data.value, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldUnsigned field");
  } else if (0 == strcmp("testFieldDouble", name)) {
    auto res = JsonHelper::Parse(mTestFieldDouble, data.value, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldDouble field");
  } else if (0 == strcmp("testFieldBool", name)) {
    auto res = JsonHelper::Parse(mTestFieldBool, data.value, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldBool field");
  } else if (0 == strcmp("testFieldFunc", name)) {
    auto res = JsonHelper::Parse(mLuaFuncHandle_testFieldFunc, data.value, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldFunc");
  } else {
    HOLGEN_WARN("Unexpected entry in json when parsing TestData: {}", name);
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
HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing TestData");
for(const auto& data: json.GetObject()) {
  const auto& name = data.name.GetString();
  if (0 == strcmp("testFieldUnsigned", name)) {
    std::string temp;
    auto res = JsonHelper::Parse(temp, data.value, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldUnsigned field");
    mTestFieldUnsigned = converter.testU32Converter(temp);
  } else if (0 == strcmp("testFieldString", name)) {
    bool temp;
    auto res = JsonHelper::Parse(temp, data.value, converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldString field");
    mTestFieldString = std::move(converter.testStringConverter(temp));
  } else {
    HOLGEN_WARN("Unexpected entry in json when parsing TestData: {}", name);
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
  HOLGEN_WARN("Could not json-parse TestData enum: invalid json type");
  return false;
}
return true;
    )R");
  }
}
