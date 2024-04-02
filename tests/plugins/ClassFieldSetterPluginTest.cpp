#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldSetterPlugin.h"

class ClassFieldSetterPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project).Run();
    ClassFieldPlugin(project).Run();
    ClassFieldSetterPlugin(project).Run();
  }
};

TEST_F(ClassFieldSetterPluginTest, Primitives) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned = 42;
  double testFieldDouble;
})R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldUnsigned", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldUnsigned", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"uint32_t", PassByType::Value, Constness::NotConst});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldUnsigned", Constness::NotConst),
                         method, "mTestFieldUnsigned = val;");
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldUnsigned", Constness::Const), nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldDouble", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldDouble", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"double", PassByType::Value, Constness::NotConst});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldDouble", Constness::NotConst), method, "mTestFieldDouble = val;");
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
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldString", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldString", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldString", Constness::NotConst), method, "mTestFieldString = val;");
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldString", Constness::Const), nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldStruct", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"InnerStruct", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldStruct", Constness::NotConst), method, "mTestFieldStruct = val;");
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldStruct", Constness::Const), nullptr);
}

TEST_F(ClassFieldSetterPluginTest, UserData) {
  auto project = Parse(R"R(
struct TestData {
  userdata testFieldUserData;
  })R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldUserData", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"SetTestFieldUserData", Type{"void"}, Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"T", PassByType::Pointer});
    method.mTemplateParameters.emplace_back("typename", "T");
    helpers::ExpectEqual(*cls->GetMethod("SetTestFieldUserData", Constness::NotConst), method,
                         "mTestFieldUserData = reinterpret_cast<void*>(val);");
  }
}

TEST_F(ClassFieldSetterPluginTest, RefWithId) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
}
struct TestData {
  Ref<InnerStruct> testFieldStruct;
  })R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldStructId", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "SetTestFieldStructId",
        Type{"void"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"uint32_t"});
    helpers::ExpectEqual(
        *cls->GetMethod("SetTestFieldStructId", Constness::NotConst),
        method, "mTestFieldStructId = val;");
  }
  ASSERT_EQ(cls->GetMethod("SetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("SetTestFieldStruct", Constness::NotConst), nullptr);
}

TEST_F(ClassFieldSetterPluginTest, ManagedRefWithId) {
  auto project = Parse(R"R(
@managed(by=DM, field=innerStructs)
struct InnerStruct {
  @id
  u32 id;
}
struct DM {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
struct TestData {
  Ref<InnerStruct> testFieldStruct;
  })R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("SetTestFieldStructId", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "SetTestFieldStructId",
        Type{"void"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"uint32_t"});
    helpers::ExpectEqual(
        *cls->GetMethod("SetTestFieldStructId", Constness::NotConst),
        method, "mTestFieldStructId = val;");
  }

  ASSERT_EQ(cls->GetMethod("SetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("SetTestFieldStruct", Constness::NotConst), nullptr);
}

TEST_F(ClassFieldSetterPluginTest, RefNoId) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  Ref<InnerStruct> testFieldStruct;
  })R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_EQ(cls->GetMethod("SetTestFieldStructId", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("SetTestFieldStructId", Constness::NotConst), nullptr);

  ASSERT_EQ(cls->GetMethod("SetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("SetTestFieldStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "SetTestFieldStruct",
        Type{"void"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("val", Type{"InnerStruct", PassByType::Pointer, Constness::NotConst});
    helpers::ExpectEqual(
        *cls->GetMethod("SetTestFieldStruct", Constness::NotConst),
        method, "mTestFieldStruct = val;");
  }
}
