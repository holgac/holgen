#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ContainerFieldPlugin.h"
#include "../Helpers.h"

class ContainerFieldPluginTest : public TranslatorPluginTest {

};

TEST_F(ContainerFieldPluginTest, Index) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  u64 uuid;
  string guid;
}
struct TestData {
  @index(on=uuid)
  @index(on=guid, using=unordered_map)
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetField("mInnerStructsUuidIndex"), nullptr);
  {
    auto field = ClassField{
        "mInnerStructsUuidIndex",
        Type{"std::map"},
    };
    field.mType.mTemplateParameters.emplace_back("uint64_t");
    field.mType.mTemplateParameters.emplace_back("uint32_t");
    helpers::ExpectEqual(*cls->GetField("mInnerStructsUuidIndex"), field);
  }
  ASSERT_NE(cls->GetField("mInnerStructsGuidIndex"), nullptr);
  {
    auto field = ClassField{
        "mInnerStructsGuidIndex",
        Type{"std::unordered_map"},
    };
    field.mType.mTemplateParameters.emplace_back("std::string");
    field.mType.mTemplateParameters.emplace_back("uint32_t");
    helpers::ExpectEqual(*cls->GetField("mInnerStructsGuidIndex"), field);
  }
}

TEST_F(ContainerFieldPluginTest, VectorIndexGetter) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  u64 uuid;
  string guid;
}
struct TestData {
  @index(on=uuid)
  @index(on=guid, using=unordered_map)
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetInnerStructFromUuid", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetInnerStructFromUuid", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStructFromUuid",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
    };
    method.mExposeToLua = true;
    const char *body = R"R(
auto it = mInnerStructsUuidIndex.find(key);
if (it == mInnerStructsUuidIndex.end())
  return nullptr;
return &mInnerStructs[it->second];
    )R";
    method.mArguments.emplace_back("key", Type{"uint64_t"});
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromUuid", Constness::Const), method, body);
    method.mExposeToLua = false;
    method.mReturnType.mConstness = Constness::NotConst;
    method.mConstness = Constness::NotConst;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromUuid", Constness::NotConst), method, body);
  }

  ASSERT_NE(cls->GetMethod("GetInnerStructFromGuid", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetInnerStructFromGuid", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStructFromGuid",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
    };
    method.mExposeToLua = true;
    const char *body = R"R(
auto it = mInnerStructsGuidIndex.find(key);
if (it == mInnerStructsGuidIndex.end())
  return nullptr;
return &mInnerStructs[it->second];
    )R";
    method.mArguments.emplace_back("key", Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromGuid", Constness::Const), method, body);
    method.mExposeToLua = false;
    method.mReturnType.mConstness = Constness::NotConst;
    method.mConstness = Constness::NotConst;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromGuid", Constness::NotConst), method, body);
  }
}

TEST_F(ContainerFieldPluginTest, MapIndexGetter) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  u64 uuid;
  string guid;
}
struct TestData {
  @index(on=uuid)
  @index(on=guid, using=unordered_map)
  @container(elemName=innerStruct)
  map<Ref<InnerStruct>, InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetInnerStructFromUuid", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetInnerStructFromUuid", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStructFromUuid",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
    };
    method.mExposeToLua = true;
    const char *body = R"R(
auto it = mInnerStructsUuidIndex.find(key);
if (it == mInnerStructsUuidIndex.end())
  return nullptr;
return &mInnerStructs.at(it->second);
    )R";
    method.mArguments.emplace_back("key", Type{"uint64_t"});
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromUuid", Constness::Const), method, body);
    method.mExposeToLua = false;
    method.mReturnType.mConstness = Constness::NotConst;
    method.mConstness = Constness::NotConst;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromUuid", Constness::NotConst), method, body);
  }

  ASSERT_NE(cls->GetMethod("GetInnerStructFromGuid", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetInnerStructFromGuid", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStructFromGuid",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
    };
    method.mExposeToLua = true;
    const char *body = R"R(
auto it = mInnerStructsGuidIndex.find(key);
if (it == mInnerStructsGuidIndex.end())
  return nullptr;
return &mInnerStructs.at(it->second);
    )R";
    method.mArguments.emplace_back("key", Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromGuid", Constness::Const), method, body);
    method.mExposeToLua = false;
    method.mReturnType.mConstness = Constness::NotConst;
    method.mConstness = Constness::NotConst;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructFromGuid", Constness::NotConst), method, body);
  }
}

TEST_F(ContainerFieldPluginTest, VectorAddElemNoId) {
  auto project = Parse(R"R(
struct InnerStruct {
}
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  ASSERT_EQ(cls->GetMethod("AddInnerStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("AddInnerStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "AddInnerStruct",
        Type{"bool"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("elem", Type{"InnerStruct", PassByType::MoveReference});
    helpers::ExpectEqual(*cls->GetMethod("AddInnerStruct", Constness::NotConst), method, R"R(
mInnerStructs.emplace_back(std::forward<InnerStruct>(elem));
return true;
    )R");
  }
}

TEST_F(ContainerFieldPluginTest, VectorAddElemWithId) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
}
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  ASSERT_EQ(cls->GetMethod("AddInnerStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("AddInnerStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "AddInnerStruct",
        Type{"bool"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("elem", Type{"InnerStruct", PassByType::MoveReference});
    helpers::ExpectEqual(*cls->GetMethod("AddInnerStruct", Constness::NotConst), method, R"R(
auto newId = mInnerStructs.size();
elem.SetId(newId);
mInnerStructs.emplace_back(std::forward<InnerStruct>(elem));
return true;
    )R");
  }
}

TEST_F(ContainerFieldPluginTest, VectorAddElemWithIdWithIndex) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  string uuid;
  string guid;
}
struct TestData {
  @index(on=uuid)
  @index(on=guid)
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_EQ(cls->GetMethod("AddInnerStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("AddInnerStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "AddInnerStruct",
        Type{"bool"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("elem", Type{"InnerStruct", PassByType::MoveReference});
    helpers::ExpectEqual(*cls->GetMethod("AddInnerStruct", Constness::NotConst), method, R"R(
if (mInnerStructsUuidIndex.contains(elem.GetUuid())) {
  HOLGEN_WARN("InnerStruct with uuid={} already exists!", elem.GetUuid());
  return false;
}
if (mInnerStructsGuidIndex.contains(elem.GetGuid())) {
  HOLGEN_WARN("InnerStruct with guid={} already exists!", elem.GetGuid());
  return false;
}
auto newId = mInnerStructs.size();
mInnerStructsUuidIndex.emplace(elem.GetUuid(), newId);
mInnerStructsGuidIndex.emplace(elem.GetGuid(), newId);
elem.SetId(newId);
mInnerStructs.emplace_back(std::forward<InnerStruct>(elem));
return true;
    )R");
  }
}

TEST_F(ContainerFieldPluginTest, MapAddElemWithId) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
  string uuid;
}
struct TestData {
  @index(on=uuid)
  @container(elemName=innerStruct)
  map<Ref<InnerStruct>, InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetField("mInnerStructs"), nullptr);
  {
    auto field = ClassField{
        "mInnerStructs",
        Type{"std::map"},
    };
    field.mType.mTemplateParameters.emplace_back("uint32_t");
    field.mType.mTemplateParameters.emplace_back("InnerStruct");
    field.mField = project.mProject.GetStruct("TestData")->GetField("innerStructs");
    helpers::ExpectEqual(*cls->GetField("mInnerStructs"), field);
  }

  ASSERT_NE(cls->GetField("mInnerStructsNextId"), nullptr);
  {
    auto field = ClassField{
        "mInnerStructsNextId",
        Type{"uint32_t"},
    };
    field.mDefaultValue = "0";
    helpers::ExpectEqual(*cls->GetField("mInnerStructsNextId"), field);
  }

  ASSERT_EQ(cls->GetMethod("AddInnerStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("AddInnerStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "AddInnerStruct",
        Type{"bool"},
        Visibility::Public, Constness::NotConst};
    method.mArguments.emplace_back("elem", Type{"InnerStruct", PassByType::MoveReference});
    helpers::ExpectEqual(*cls->GetMethod("AddInnerStruct", Constness::NotConst), method, R"R(
if (mInnerStructsUuidIndex.contains(elem.GetUuid())) {
  HOLGEN_WARN("InnerStruct with uuid={} already exists!", elem.GetUuid());
  return false;
}
auto newId = mInnerStructsNextId;
++mInnerStructsNextId;
mInnerStructsUuidIndex.emplace(elem.GetUuid(), newId);
elem.SetId(newId);
mInnerStructs.emplace(newId, std::forward<InnerStruct>(elem));
return true;
    )R");
  }
}

TEST_F(ContainerFieldPluginTest, VectorGetElemNoId) {
  auto project = Parse(R"R(
struct InnerStruct {
}
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  ASSERT_NE(cls->GetMethod("GetInnerStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetInnerStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStruct",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
        Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("idx", Type{"size_t"});
    method.mExposeToLua = true;
    const char *body = R"R(
if (idx >= mInnerStructs.size())
  return nullptr;
return &mInnerStructs[idx];
)R";
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStruct", Constness::Const), method, body);
    method.mReturnType.mConstness = Constness::NotConst;
    method.mConstness = Constness::NotConst;
    method.mExposeToLua = false;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStruct", Constness::NotConst), method, body);
  }
}

TEST_F(ContainerFieldPluginTest, VectorGetElemWithSignedId) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  s32 id;
}
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);
  EXPECT_EQ(cls->mFields.size(), 1);

  ASSERT_NE(cls->GetMethod("GetInnerStruct", Constness::Const), nullptr);
  ASSERT_NE(cls->GetMethod("GetInnerStruct", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStruct",
        Type{"InnerStruct", PassByType::Pointer, Constness::Const},
        Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    method.mExposeToLua = true;
    const char *body = R"R(
if (idx >= mInnerStructs.size() || idx < 0)
  return nullptr;
return &mInnerStructs[idx];
    )R";
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStruct", Constness::Const), method, body);
    method.mReturnType.mConstness = Constness::NotConst;
    method.mConstness = Constness::NotConst;
    method.mExposeToLua = false;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStruct", Constness::NotConst), method, body);
  }
}

TEST_F(ContainerFieldPluginTest, VectorGetCount) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  s32 id;
}
struct TestData {
  @container(elemName=innerStruct)
  vector<InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetInnerStructCount", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("GetInnerStructCount", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStructCount",
        Type{"size_t"},
        Visibility::Public, Constness::Const};
    method.mExposeToLua = true;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructCount", Constness::Const), method, R"R(
return mInnerStructs.size();
    )R");
  }
}

TEST_F(ContainerFieldPluginTest, MapGetCount) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  s32 id;
}
struct TestData {
  @container(elemName=innerStruct)
  map<Ref<InnerStruct>, InnerStruct> innerStructs;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ContainerFieldPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("GetInnerStructCount", Constness::Const), nullptr);
  ASSERT_EQ(cls->GetMethod("GetInnerStructCount", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "GetInnerStructCount",
        Type{"size_t"},
        Visibility::Public, Constness::Const};
    method.mExposeToLua = true;
    helpers::ExpectEqual(*cls->GetMethod("GetInnerStructCount", Constness::Const), method, R"R(
return mInnerStructs.size();
    )R");
  }
}
