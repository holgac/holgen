#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassIdFieldPlugin.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/CppFunctionPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "core/St.h"

class LuaPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassIdFieldPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
    CppFunctionPlugin(project, {}).Run();
    LuaPlugin(project, {}).Run();
  }
};

TEST_F(LuaPluginTest, Enum) {
  auto project = Parse(R"R(
enum TestData {}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushToLua", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushToLua", Constness::Const), method,
                         "LuaHelper::Push(mValue, luaState, true);");
  }
}

TEST_F(LuaPluginTest, PushToLuaNotManaged) {
  auto project = Parse(R"R(
struct TestData {}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushToLua", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"PushToLua", Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushToLua", Constness::Const), method, R"R(
lua_newtable(luaState);
lua_pushstring(luaState, "p");
lua_pushlightuserdata(luaState, (void *) this);
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
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushGlobalToLua", Constness::Const), nullptr);
  {
    auto method =
        ClassMethod{"PushGlobalToLua", Type{"void"}, Visibility::Public, Constness::Const};
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
  Run(project);
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
lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
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
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod(St::Lua_ReadProxyObject, Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{St::Lua_ReadProxyObject, Type{"TestData", PassByType::Pointer},
                              Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    helpers::ExpectEqual(*cls->GetMethod(St::Lua_ReadProxyObject, Constness::NotConst), method, R"R(
lua_pushstring(luaState, "p");
lua_gettable(luaState, idx - 1);
auto ptr = (TestData *) lua_touserdata(luaState, -1);
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
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod(St::Lua_ReadProxyObject, Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{St::Lua_ReadProxyObject, Type{"TestData", PassByType::Pointer},
                              Visibility::Public, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("idx", Type{"int32_t"});
    helpers::ExpectEqual(*cls->GetMethod(St::Lua_ReadProxyObject, Constness::NotConst), method, R"R(
lua_pushstring(luaState, "i");
lua_gettable(luaState, idx - 1);
uint32_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
auto ptr = TestData::Get(id);
lua_pop(luaState, 1);
return ptr;
    )R");
  }
}

TEST_F(LuaPluginTest, IndexMetaMethodFields) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  u32 testFieldUnsigned;
  string testFieldString;
  bool testFieldBool;
  InnerStruct testFieldInnerStruct;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("IndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"IndexMetaMethod", Type{"int"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("IndexMetaMethod", Constness::NotConst), method, R"R(
auto instance = TestData::ReadProxyFromLua(luaState, -2);
const char *key = lua_tostring(luaState, -1);
if (0 == strcmp("testFieldUnsigned", key)) {
  LuaHelper::Push(instance->mTestFieldUnsigned, luaState, false);
} else if (0 == strcmp("testFieldString", key)) {
  LuaHelper::Push(instance->mTestFieldString, luaState, false);
} else if (0 == strcmp("testFieldBool", key)) {
  LuaHelper::Push(instance->mTestFieldBool, luaState, false);
} else if (0 == strcmp("testFieldInnerStruct", key)) {
  LuaHelper::Push(instance->mTestFieldInnerStruct, luaState, false);
} else {
  HOLGEN_WARN("Unexpected lua field: TestData.{}", key);
  return 0;
}
return 1;
    )R");
  }
}

TEST_F(LuaPluginTest, IndexMetaMethodFunctions) {
  auto project = Parse(R"R(
struct TestData {
  func functionReturningVoid(s32 a0, string a1) -> void;
  func functionReturningString() -> string;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("IndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"IndexMetaMethod", Type{"int"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("IndexMetaMethod", Constness::NotConst), method, R"R(
const char *key = lua_tostring(luaState, -1);
if (0 == strcmp("functionReturningVoid", key)) {
  lua_pushcfunction(luaState, [](lua_State *lsInner) {
    auto instance = TestData::ReadProxyFromLua(lsInner, -3);
    int32_t arg0;
    LuaHelper::Read(arg0, lsInner, -2);
    std::string arg1;
    LuaHelper::Read(arg1, lsInner, -1);
    instance->functionReturningVoid(arg0, arg1);
    return 0;
  });
} else if (0 == strcmp("functionReturningString", key)) {
  lua_pushcfunction(luaState, [](lua_State *lsInner) {
    auto instance = TestData::ReadProxyFromLua(lsInner, -1);
    auto result = instance->functionReturningString();
    LuaHelper::Push(result, lsInner, false);
    return 1;
  });
} else {
  HOLGEN_WARN("Unexpected lua field: TestData.{}", key);
  return 0;
}
return 1;
  )R");
  }
}

TEST_F(LuaPluginTest, IndexMetaMethodRefs) {
  auto project = Parse(R"R(
struct InnerStructWithId {
  @id
  u32 id;
}
struct InnerStructNoId {}
struct TestData {
  Ref<InnerStructWithId> testStructWithIdRef;
  Ref<InnerStructNoId> testStructNoIdRef;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("IndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"IndexMetaMethod", Type{"int"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("IndexMetaMethod", Constness::NotConst), method, R"R(
auto instance = TestData::ReadProxyFromLua(luaState, -2);
const char *key = lua_tostring(luaState, -1);
if (0 == strcmp("testStructWithIdRefId", key)) {
  LuaHelper::Push(instance->mTestStructWithIdRefId, luaState, false);
} else if (0 == strcmp("testStructNoIdRef", key)) {
  LuaHelper::Push(instance->mTestStructNoIdRef, luaState, false);
} else {
  HOLGEN_WARN("Unexpected lua field: TestData.{}", key);
  return 0;
}
return 1;
    )R");
  }
}

TEST_F(LuaPluginTest, NewIndexMetaMethodFields) {
  auto project = Parse(R"R(
struct TestData {
  u32 testFieldUnsigned;
  string testFieldString;
  bool testFieldBool;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("NewIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"NewIndexMetaMethod", Type{"int"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("NewIndexMetaMethod", Constness::NotConst), method, R"R(
auto instance = TestData::ReadProxyFromLua(luaState, -3);
const char *key = lua_tostring(luaState, -2);
if (0 == strcmp("testFieldUnsigned", key)) {
  LuaHelper::Read(instance->mTestFieldUnsigned, luaState, -1);
} else if (0 == strcmp("testFieldString", key)) {
  LuaHelper::Read(instance->mTestFieldString, luaState, -1);
} else if (0 == strcmp("testFieldBool", key)) {
  LuaHelper::Read(instance->mTestFieldBool, luaState, -1);
} else {
  HOLGEN_WARN("Unexpected lua field: TestData.{}", key);
}
return 0;
    )R");
  }
}

TEST_F(LuaPluginTest, NewIndexMetaMethodRefs) {
  auto project = Parse(R"R(
struct InnerStruct {
  @id
  u32 id;
}
struct TestData {
  Ref<InnerStruct> testStructRef;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("NewIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"NewIndexMetaMethod", Type{"int"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("NewIndexMetaMethod", Constness::NotConst), method, R"R(
auto instance = TestData::ReadProxyFromLua(luaState, -3);
const char *key = lua_tostring(luaState, -2);
if (0 == strcmp("testStructRefId", key)) {
  LuaHelper::Read(instance->mTestStructRefId, luaState, -1);
} else {
  HOLGEN_WARN("Unexpected lua field: TestData.{}", key);
}
return 0;
    )R");
  }
}

TEST_F(LuaPluginTest, CreateLuaMetatable) {
  auto project = Parse(R"R(
struct TestData {}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("CreateLuaMetatable", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"CreateLuaMetatable", Type{"void"}, Visibility::Public,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("CreateLuaMetatable", Constness::NotConst), method, R"R(
lua_newtable(luaState);
lua_pushstring(luaState, "__index");
lua_pushcfunction(luaState, TestData::IndexMetaMethod);
lua_settable(luaState, -3);
lua_pushstring(luaState, "__newindex");
lua_pushcfunction(luaState, TestData::NewIndexMetaMethod);
lua_settable(luaState, -3);
lua_setglobal(luaState, "TestDataMeta");
    )R");
  }
}
