#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldSetterPlugin.h"
#include "../Helpers.h"

class ClassFieldSetterPluginTest : public TranslatorPluginTest {
protected:
};

TEST_F(ClassFieldSetterPluginTest, Primitives) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned = 42;
  double testFieldDouble;
})R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldSetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldUnsigned", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldUnsigned", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mBody.Add("mTestFieldUnsigned = val;");
    method.mArguments.emplace_back("val", Type{"uint32_t", PassByType::Value, Constness::NotConst});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldUnsigned", Constness::NotConst), method);
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldUnsigned", Constness::Const), nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldDouble", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldDouble", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mBody.Add("mTestFieldDouble = val;");
    method.mArguments.emplace_back("val", Type{"double", PassByType::Value, Constness::NotConst});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldDouble", Constness::NotConst), method);
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldDouble", Constness::Const), nullptr);
}

TEST_F(ClassFieldSetterPluginTest, NonPrimitives) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  string testFieldString;
  InnerStruct testFieldStruct;
})R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldSetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldString", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldString", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mBody.Add("mTestFieldString = val;");
    method.mArguments.emplace_back("val", Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldString", Constness::NotConst), method);
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldString", Constness::Const), nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldStruct", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mBody.Add("mTestFieldStruct = val;");
    method.mArguments.emplace_back("val", Type{"InnerStruct", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldStruct", Constness::NotConst), method);
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldStruct", Constness::Const), nullptr);
}