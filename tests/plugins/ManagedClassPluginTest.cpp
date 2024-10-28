#include "TranslatorPluginTest.h"
#include "generator/plugins/cpp/ClassFieldPlugin.h"
#include "generator/plugins/cpp/ClassIdFieldPlugin.h"
#include "generator/plugins/cpp/ClassPlugin.h"
#include "generator/plugins/cpp/ManagedClassPlugin.h"

class ManagedClassPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
    ManagedClassPlugin(project, {}).Run();
  }
};

TEST_F(ManagedClassPluginTest, Getters) {
  auto project = Parse(R"R(
@managed(by=DM, field=data)
struct TestData {
  @id
  u32 id;
  u64 uuid;
  string guid;
}
@dataManager
struct DM {
  @index(on=uuid)
  @index(on=guid)
  @container(elemName=datum)
  vector<TestData> data;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("Get", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"Get", Type{"TestData", PassByType::Pointer, Constness::NotConst},
                              Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("id", Type{"uint32_t"});
    helpers::ExpectEqual(*cls->GetMethod("Get", Constness::NotConst), method,
                         "return GlobalPointer<DM>::GetInstance()->GetDatum(id);");
  }

  ASSERT_NE(cls->GetMethod("GetFromUuid", Constness::NotConst), nullptr);
  {
    auto method =
        ClassMethod{"GetFromUuid", Type{"TestData", PassByType::Pointer, Constness::NotConst},
                    Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("key", Type{"uint64_t"});
    helpers::ExpectEqual(*cls->GetMethod("GetFromUuid", Constness::NotConst), method,
                         "return GlobalPointer<DM>::GetInstance()->GetDatumFromUuid(key);");
  }

  ASSERT_NE(cls->GetMethod("GetFromGuid", Constness::NotConst), nullptr);
  {
    auto method =
        ClassMethod{"GetFromGuid", Type{"TestData", PassByType::Pointer, Constness::NotConst},
                    Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("key",
                                   Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("GetFromGuid", Constness::NotConst), method,
                         "return GlobalPointer<DM>::GetInstance()->GetDatumFromGuid(key);");
  }
}

TEST_F(ManagedClassPluginTest, InvalidManagedAnnotation) {
  ExpectErrorMessage("@managed(field) struct TestData {}", Run,
                     "TestData ({0}:1:17) is a managed class without an id field", Source);
  ExpectErrorMessage("@managed(field) struct TestData {@id u32 id;}", Run,
                     "Missing by attribute in managed ({0}:1:1) annotation of TestData ({0}:1:17)",
                     Source);
  ExpectErrorMessage(
      "@managed(by) struct TestData {@id u32 id;}", Run,
      "Missing field attribute in managed ({0}:1:1) annotation of TestData ({0}:1:14)", Source);
  ExpectErrorMessage("@managed(by=DM, field=data) struct TestData {@id u32 id;}", Run,
                     "TestData ({0}:1:29) is managed by DM which does not exist", Source);
  ExpectErrorMessage(
      R"R(
@managed(by=DM, field=data) struct TestData {@id u32 id;}
struct DM {}
  )R",
      Run, "TestData ({0}:1:29) is managed by DM ({0}:2:1) which is not a data manager", Source);
  ExpectErrorMessage(
      R"R(
@managed(by=DM, field=data) struct TestData {@id u32 id;}
@dataManager
struct DM {}
  )R",
      Run, "TestData ({0}:1:29) is managed by data field of DM ({0}:3:1) which does not exist",
      Source);
  ExpectErrorMessage(
      R"R(
@managed(by=DM, field=data) struct TestData {@id u32 id;}
@dataManager
struct DM {
  vector<TestData> data;
}
  )R",
      Run, "TestData ({0}:1:29) is managed by DM.data ({0}:4:3) which is not a container", Source);
  ExpectErrorMessage(
      R"R(
@managed(by=DM, field=data) struct TestData {@id u32 id;}
@dataManager
struct DM {
  @container(elemName=datum)
  vector<Another> data;
}
struct Another {}
  )R",
      Run,
      "TestData ({0}:1:29) is managed by DM.data ({0}:5:3) which is not a container of TestData",
      Source);
}
