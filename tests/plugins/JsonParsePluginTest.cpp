#include "TranslatorPluginTest.h"
#include "generator/plugins/cpp/ClassFieldPlugin.h"
#include "generator/plugins/cpp/ClassIdFieldPlugin.h"
#include "generator/plugins/cpp/ClassPlugin.h"
#include "generator/plugins/json/JsonConverterPlugin.h"
#include "generator/plugins/json/JsonParsePlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"

class JsonParsePluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    TranslatorSettings translatorSettings;
    translatorSettings.EnableFeature(TranslatorFeatureFlag::Json);
    translatorSettings.EnableFeature(TranslatorFeatureFlag::Lua);
    ClassPlugin(project, translatorSettings).Run();
    ClassIdFieldPlugin(project, translatorSettings).Run();
    ClassFieldPlugin(project, translatorSettings).Run();
    JsonConverterPlugin(project, translatorSettings).Run();
    // Not a direct dependency, but needed to parse lua function names
    LuaFunctionPlugin(project, translatorSettings).Run();
    JsonParsePlugin(project, translatorSettings).Run();
  }
};

TEST_F(JsonParsePluginTest, StructParseJsonEmpty) {
  auto project = Parse("struct TestData {}");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseJson", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseJson", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back(
        "json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter",
                                   Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
return true;
    )R");
  }
}

TEST_F(JsonParsePluginTest, StructParseJson) {
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
    method.mArguments.emplace_back(
        "json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter",
                                   Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
if (json.IsObject()) {
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
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
} else if (json.IsArray()) {
  auto it = json.Begin();
  {
    HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestData!");
    auto res = JsonHelper::Parse(mTestFieldUnsigned, (*it), converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldUnsigned field");
    ++it;
  }
  {
    HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestData!");
    auto res = JsonHelper::Parse(mTestFieldDouble, (*it), converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldDouble field");
    ++it;
  }
  {
    HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestData!");
    auto res = JsonHelper::Parse(mTestFieldBool, (*it), converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldBool field");
    ++it;
  }
  {
    auto res = JsonHelper::Parse(mLuaFuncHandle_testFieldFunc, (*it), converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldFunc");
    ++it;
  }
  HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestData!");
} else {
  HOLGEN_WARN("Unexpected json type when parsing TestData.");
  return false;
}
return true;
    )R");
  }
}

TEST_F(JsonParsePluginTest, StructParseJsonConverter) {
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
    method.mArguments.emplace_back(
        "json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter",
                                   Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
if (json.IsObject()) {
  for (const auto &data: json.GetObject()) {
    const auto &name = data.name.GetString();
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
} else if (json.IsArray()) {
  auto it = json.Begin();
  {
    HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestData!");
    std::string temp;
    auto res = JsonHelper::Parse(temp, (*it), converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldUnsigned field");
    mTestFieldUnsigned = converter.testU32Converter(temp);
    ++it;
  }
  {
    HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestData!");
    bool temp;
    auto res = JsonHelper::Parse(temp, (*it), converter);
    HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestData.testFieldString field");
    mTestFieldString = std::move(converter.testStringConverter(temp));
    ++it;
  }
  HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestData!");
} else {
  HOLGEN_WARN("Unexpected json type when parsing TestData.");
  return false;
}
return true;
    )R");
  }
}

TEST_F(JsonParsePluginTest, EnumParseJson) {
  auto project = Parse("enum TestData {}");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ParseJson", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ParseJson", Type{"bool"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back(
        "json", Type{"rapidjson::Value", PassByType::Reference, Constness::Const});
    method.mArguments.emplace_back("converter",
                                   Type{"Converter", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("ParseJson", Constness::NotConst), method, R"R(
if (json.IsString()) {
  *this = TestData::FromString(std::string_view(json.GetString(), json.GetStringLength()));
} else if (json.IsInt64()) {
  *this = TestData(json.GetInt64());
} else {
  *this = TestData{};
  HOLGEN_WARN("Could not json-parse TestData enum: invalid json type");
  return false;
}
return true;
    )R");
  }
}
