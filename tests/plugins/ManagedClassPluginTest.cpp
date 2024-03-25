#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ManagedClassPlugin.h"
#include "../Helpers.h"

class ManagedClassPluginTest : public TranslatorPluginTest {

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
struct DM {
  @index(on=uuid)
  @index(on=guid)
  @container(elemName=datum)
  vector<TestData> data;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  ManagedClassPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("Get", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
        "Get",
        Type{"TestData", PassByType::Pointer, Constness::NotConst},
        Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("id", Type{"uint32_t"});
    helpers::ExpectEqual(*cls->GetMethod("Get", Constness::NotConst), method,
                         "return GlobalPointer<DM>::GetInstance()->GetDatum(id);");
  }

  ASSERT_NE(cls->GetMethod("GetFromUuid", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
      "GetFromUuid",
      Type{"TestData", PassByType::Pointer, Constness::NotConst},
      Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("key", Type{"uint64_t"});
    helpers::ExpectEqual(*cls->GetMethod("GetFromUuid", Constness::NotConst), method,
                         "return GlobalPointer<DM>::GetInstance()->GetDatumFromUuid(key);");
  }

  ASSERT_NE(cls->GetMethod("GetFromGuid", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{
      "GetFromGuid",
      Type{"TestData", PassByType::Pointer, Constness::NotConst},
      Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("key", Type{"std::string", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("GetFromGuid", Constness::NotConst), method,
                         "return GlobalPointer<DM>::GetInstance()->GetDatumFromGuid(key);");
  }
}
