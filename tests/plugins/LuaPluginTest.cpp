#include "TranslatorPluginTest.h"
#include "generator/plugins/cpp/ClassFieldPlugin.h"
#include "generator/plugins/cpp/ClassIdFieldPlugin.h"
#include "generator/plugins/cpp/ClassPlugin.h"
#include "generator/plugins/cpp/CppFunctionPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"
#include "generator/plugins/lua/LuaIndexMetaMethodPlugin.h"
#include "generator/plugins/enum/EnumPlugin.h"
#include "core/St.h"

class LuaPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    TranslatorSettings translatorSettings;
    translatorSettings.EnableFeature(TranslatorFeatureFlag::Lua);
    ClassPlugin(project, translatorSettings).Run();
    EnumPlugin(project, translatorSettings).Run();
    ClassIdFieldPlugin(project, translatorSettings).Run();
    ClassFieldPlugin(project, translatorSettings).Run();
    CppFunctionPlugin(project, translatorSettings).Run();
    LuaPlugin(project, translatorSettings).Run();
    LuaIndexMetaMethodPlugin(project, translatorSettings).Run();
  }
};

TEST_F(LuaPluginTest, Enum) {
  auto project = Parse(R"R(
enum TestData {}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod(St::Lua_PushProxyObject, Constness::Const), nullptr);
  {
    auto method =
        ClassMethod{St::Lua_PushProxyObject, Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod(St::Lua_PushProxyObject, Constness::Const), method,
                         "LuaHelper::Push<true>(TestData::UnderlyingType(mValue), luaState);");
  }
}

TEST_F(LuaPluginTest, PushToLuaNotManaged) {
  auto project = Parse(R"R(
struct TestData {}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod(St::Lua_PushProxyObject, Constness::Const), nullptr);
  {
    auto method =
        ClassMethod{St::Lua_PushProxyObject, Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod(St::Lua_PushProxyObject, Constness::Const), method, R"R(
lua_newtable(luaState);
lua_pushstring(luaState, "p");
lua_pushlightuserdata(luaState, (void *) this);
lua_settable(luaState, -3);
lua_pushstring(luaState, "c");
lua_pushlightuserdata(luaState, &CLASS_NAME);
lua_settable(luaState, -3);
lua_getglobal(luaState, "TestData");
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

  ASSERT_NE(cls->GetMethod(St::Lua_PushProxyObject, Constness::Const), nullptr);
  {
    auto method =
        ClassMethod{St::Lua_PushProxyObject, Type{"void"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod(St::Lua_PushProxyObject, Constness::Const), method, R"R(
lua_newtable(luaState);
uint64_t id = mId;
lua_pushstring(luaState, "i");
lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
lua_settable(luaState, -3);
lua_pushstring(luaState, "c");
lua_pushlightuserdata(luaState, &CLASS_NAME);
lua_settable(luaState, -3);
lua_getglobal(luaState, "TestData");
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
lua_pushstring(luaState, "c");
lua_gettable(luaState, idx - 1);
if (!lua_isuserdata(luaState, -1)) {
  HOLGEN_WARN("Proxy object does not contain the correct metadata!");
  return nullptr;
}
auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
lua_pop(luaState, 1);
HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestData", className);
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
lua_pushstring(luaState, "c");
lua_gettable(luaState, idx - 1);
if (!lua_isuserdata(luaState, -1)) {
  HOLGEN_WARN("Proxy object does not contain the correct metadata!");
  return nullptr;
}
auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
lua_pop(luaState, 1);
HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestData", className);
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
const char *key = lua_tostring(luaState, -1);
if (0 == strcmp("testFieldUnsigned", key)) {
  auto instance = TestData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestData.testFieldUnsigned with an invalid lua proxy object!");
  LuaHelper::Push<false>(instance->mTestFieldUnsigned, luaState);
} else if (0 == strcmp("testFieldString", key)) {
  auto instance = TestData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestData.testFieldString with an invalid lua proxy object!");
  LuaHelper::Push<false>(instance->mTestFieldString, luaState);
} else if (0 == strcmp("testFieldBool", key)) {
  auto instance = TestData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestData.testFieldBool with an invalid lua proxy object!");
  LuaHelper::Push<false>(instance->mTestFieldBool, luaState);
} else if (0 == strcmp("testFieldInnerStruct", key)) {
  auto instance = TestData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestData.testFieldInnerStruct with an invalid lua proxy object!");
  LuaHelper::Push<false>(instance->mTestFieldInnerStruct, luaState);
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
  lua_pushcfunction(luaState, TestData::functionReturningVoidCallerFromLua);
} else if (0 == strcmp("functionReturningString", key)) {
  lua_pushcfunction(luaState, TestData::functionReturningStringCallerFromLua);
} else {
  HOLGEN_WARN("Unexpected lua field: TestData.{}", key);
  return 0;
}
return 1;
  )R");
  }

  ASSERT_NE(cls->GetMethod("functionReturningVoidCallerFromLua", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"functionReturningVoidCallerFromLua", Type{"int"},
                              Visibility::Private, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("functionReturningVoidCallerFromLua", Constness::NotConst),
                         method, R"R(
auto instance = TestData::ReadProxyFromLua(luaState, -3);
HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestData.functionReturningVoid method with an invalid lua proxy object!");
int32_t arg0;
LuaHelper::Read(arg0, luaState, -2);
std::string arg1;
LuaHelper::Read(arg1, luaState, -1);
instance->functionReturningVoid(arg0, arg1);
return 0;
  )R");
  }

  ASSERT_NE(cls->GetMethod("functionReturningStringCallerFromLua", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"functionReturningStringCallerFromLua", Type{"int"},
                              Visibility::Private, Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(
        *cls->GetMethod("functionReturningStringCallerFromLua", Constness::NotConst), method, R"R(
auto instance = TestData::ReadProxyFromLua(luaState, -1);
HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestData.functionReturningString method with an invalid lua proxy object!");
auto result = instance->functionReturningString();
LuaHelper::Push<true>(result, luaState);
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
const char *key = lua_tostring(luaState, -1);
if (0 == strcmp("testStructWithIdRefId", key)) {
  auto instance = TestData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestData.testStructWithIdRef with an invalid lua proxy object!");
  LuaHelper::Push<false>(instance->mTestStructWithIdRefId, luaState);
} else if (0 == strcmp("testStructNoIdRef", key)) {
  auto instance = TestData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestData.testStructNoIdRef with an invalid lua proxy object!");
  LuaHelper::Push<false>(instance->mTestStructNoIdRef, luaState);
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
  auto res = LuaHelper::Read(instance->mTestFieldUnsigned, luaState, -1);
  HOLGEN_WARN_IF(!res, "Assigning TestData.testFieldUnsigned from lua failed!");
} else if (0 == strcmp("testFieldString", key)) {
  auto res = LuaHelper::Read(instance->mTestFieldString, luaState, -1);
  HOLGEN_WARN_IF(!res, "Assigning TestData.testFieldString from lua failed!");
} else if (0 == strcmp("testFieldBool", key)) {
  auto res = LuaHelper::Read(instance->mTestFieldBool, luaState, -1);
  HOLGEN_WARN_IF(!res, "Assigning TestData.testFieldBool from lua failed!");
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
  auto res = LuaHelper::Read(instance->mTestStructRefId, luaState, -1);
  HOLGEN_WARN_IF(!res, "Assigning TestData.testStructRef from lua failed!");
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
lua_setglobal(luaState, "TestData");
    )R");
  }
}
