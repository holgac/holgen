#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "../Helpers.h"

class LuaPluginTest : public TranslatorPluginTest {
};

TEST_F(LuaPluginTest, Enum) {
  auto project = Parse(R"R(
enum TestData {}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  LuaPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushToLua", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(
        *cls->GetMethod("PushToLua", Constness::Const),
        method, "LuaHelper::Push(mValue, luaState);");
  }
}

TEST_F(LuaPluginTest, PushToLuaNotManaged) {
  auto project = Parse(R"R(
struct TestData {}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  LuaPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushToLua", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushToLua", Constness::Const), method, R"R(
lua_newtable(luaState);
lua_pushstring(luaState, "p");
lua_pushlightuserdata(luaState, (void*)this);
lua_settable(luaState, -3);
lua_getglobal(luaState, "TestDataMeta");
lua_setmetatable(luaState, -2);
    )R");
  }
}

TEST_F(LuaPluginTest, PushGlobalToLua) {
  auto project = Parse(R"R(
struct TestData {}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  LuaPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushGlobalToLua", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"PushGlobalToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("name", Type{"char", PassByType::Pointer, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("PushGlobalToLua", Constness::Const), method, R"R(
PushToLua(luaState);
lua_setglobal(luaState, name);
    )R");
  }
}

TEST_F(LuaPluginTest, PushToLuaManaged) {
  auto project = Parse(R"R(
@managed(by=DM, field=testData)
struct TestData {
@id
u32 id;
}
@dataManager
struct DM {
  @container(elemName=testDatum)
  vector<TestData> testData;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  LuaPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushToLua", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushToLua", Constness::Const), method, R"R(
lua_newtable(luaState);
uint64_t id = mId;
lua_pushstring(luaState, "i");
lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));
lua_settable(luaState, -3);
lua_getglobal(luaState, "TestDataMeta");
lua_setmetatable(luaState, -2);
    )R");
  }
}

TEST_F(LuaPluginTest, ReadFromLuaNotManaged) {
  auto project = Parse(R"R(
struct TestData {}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  LuaPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ReadFromLua", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ReadFromLua", Type{"TestData", PassByType::Pointer}, Visibility::Public,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    helpers::ExpectEqual(*cls->GetMethod("ReadFromLua", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "p");
lua_gettable(luaState, idx - 1);
auto ptr = (TestData*)lua_touserdata(luaState, -1);
lua_pop(luaState, 1);
return ptr;
    )R");
  }
}

TEST_F(LuaPluginTest, ReadFromLuaManaged) {
  auto project = Parse(R"R(
@managed(by=DM, field=testData)
struct TestData {
@id
u32 id;
}
@dataManager
struct DM {
  @container(elemName=testDatum)
  vector<TestData> testData;
}
  )R");
  ClassPlugin(project).Run();
  ClassFieldPlugin(project).Run();
  LuaPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("ReadFromLua", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"ReadFromLua", Type{"TestData", PassByType::Pointer}, Visibility::Public,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    helpers::ExpectEqual(*cls->GetMethod("ReadFromLua", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "i");
lua_gettable(luaState, idx - 1);
uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
auto ptr = TestData::Get(id);
lua_pop(luaState, 1);
return ptr;
    )R");
  }
}
