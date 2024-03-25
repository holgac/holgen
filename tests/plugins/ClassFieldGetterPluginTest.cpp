#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassFieldGetterPlugin.h"
#include "../Helpers.h"

class ClassFieldGetterPluginTest : public TranslatorPluginTest {
protected:
};

TEST_F(ClassFieldGetterPluginTest, Primitives) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned = 42;
  double testFieldDouble;
})R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldGetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetTestFieldUnsigned", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"GetTestFieldUnsigned", Type{"uint32_t"}, Visibility::Public, Constness::Const};
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldUnsigned", Constness::Const), method,
                         "return mTestFieldUnsigned;");
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldUnsigned", Constness::NotConst), nullptr);

  ASSERT_NE(cls->GetMethod("GetTestFieldDouble", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"GetTestFieldDouble", Type{"double"}, Visibility::Public, Constness::Const};
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldDouble", Constness::Const), method, "return mTestFieldDouble;");
  }
  ASSERT_EQ(cls->GetMethod("GetTestFieldDouble", Constness::NotConst), nullptr);
}

TEST_F(ClassFieldGetterPluginTest, NonPrimitives) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  string testFieldString;
  InnerStruct testFieldStruct;
})R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldGetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetTestFieldString", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetTestFieldString", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetTestFieldString",
        Type{"std::string", PassByType::Reference, Constness::Const},
        Visibility::Public, Constness::Const};
    method.mBody.Add("return mTestFieldString;");
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldString", Constness::Const), method);

    method.mConstness = Constness::NotConst;
    method.mReturnType.mConstness = Constness::NotConst;
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldString", Constness::NotConst), method);
  }

  ASSERT_NE(cls->GetMethod("GetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetTestFieldStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetTestFieldStruct",
        Type{"InnerStruct", PassByType::Reference, Constness::Const},
        Visibility::Public, Constness::Const};
    method.mBody.Add("return mTestFieldStruct;");
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldStruct", Constness::Const), method);

    method.mConstness = Constness::NotConst;
    method.mReturnType.mConstness = Constness::NotConst;
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldStruct", Constness::NotConst), method);
  }
}

TEST_F(ClassFieldGetterPluginTest, RefWithId) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
}
struct TestData {
  Ref<InnerStruct> testFieldStruct;
})R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldGetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetTestFieldStructId", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("GetTestFieldStructId", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetTestFieldStructId",
        Type{"uint32_t"},
        Visibility::Public, Constness::Const};
    helpers::ExpectEqual(
        *cls->GetMethod("GetTestFieldStructId", Constness::Const),
        method, "return mTestFieldStructId;");
  }

  ASSERT_EQ(cls->GetMethod("GetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("GetTestFieldStruct", Constness::NotConst), nullptr);
}

TEST_F(ClassFieldGetterPluginTest, ManagedRefWithId) {
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
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldGetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetTestFieldStructId", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("GetTestFieldStructId", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetTestFieldStructId",
        Type{"uint32_t"},
        Visibility::Public, Constness::Const};
    helpers::ExpectEqual(
        *cls->GetMethod("GetTestFieldStructId", Constness::Const),
        method, "return mTestFieldStructId;");
  }

  ASSERT_NE(cls->GetMethod("GetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetTestFieldStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetTestFieldStruct",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
        Visibility::Public, Constness::Const};
    method.mBody.Add("return InnerStruct::Get(mTestFieldStructId);");
    method.mExposeToLua = true;
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldStruct", Constness::Const), method);
    method.mConstness = Constness::NotConst;
    method.mReturnType.mConstness = Constness::NotConst;
    method.mExposeToLua = false;
    helpers::ExpectEqual(*cls->GetMethod("GetTestFieldStruct", Constness::NotConst), method);
  }
}

TEST_F(ClassFieldGetterPluginTest, RefNoId) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  Ref<InnerStruct> testFieldStruct;
  })R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ClassFieldGetterPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_EQ(cls->GetMethod("GetTestFieldStructId", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("GetTestFieldStructId", Constness::NotConst), nullptr);

  ASSERT_NE(cls->GetMethod("GetTestFieldStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetTestFieldStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetTestFieldStruct",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
        Visibility::Public, Constness::Const};
    helpers::ExpectEqual(
        *cls->GetMethod("GetTestFieldStruct", Constness::Const),
        method, "return mTestFieldStruct;");
    method.mConstness = Constness::NotConst;
    method.mReturnType.mConstness = Constness::NotConst;
    helpers::ExpectEqual(
        *cls->GetMethod("GetTestFieldStruct", Constness::NotConst),
        method, "return mTestFieldStruct;");
  }
}
