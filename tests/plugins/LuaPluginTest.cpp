#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/CppFunctionPlugin.h"
#include "generator/plugins/lua/LuaPlugin.h"

class LuaPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project).Run();
    ClassFieldPlugin(project).Run();
    CppFunctionPlugin(project).Run();
    LuaPlugin(project).Run();
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
    helpers::ExpectEqual(
        *cls->GetMethod("PushToLua", Constness::Const),
        method, "LuaHelper::Push(mValue, luaState);");
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
  Run(project);
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
  Run(project);
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
  Run(project);
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

TEST_F(LuaPluginTest, PushIndexMetaMethodFields) {
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

  ASSERT_NE(cls->GetMethod("PushIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"PushIndexMetaMethod", Type{"void"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushIndexMetaMethod", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "__index");
lua_pushcfunction(luaState, [](lua_State* ls) {
  auto instance = TestData::ReadFromLua(ls, -2);
  const char* key = lua_tostring(ls, -1);
  if (0 == strcmp("testFieldUnsigned", key)) {
    LuaHelper::Push(instance->mTestFieldUnsigned, ls);
  } else if (0 == strcmp("testFieldString", key)) {
    LuaHelper::Push(instance->mTestFieldString, ls);
  } else if (0 == strcmp("testFieldBool", key)) {
    LuaHelper::Push(instance->mTestFieldBool, ls);
  } else if (0 == strcmp("testFieldInnerStruct", key)) {
    LuaHelper::Push(instance->mTestFieldInnerStruct, ls);
  } else {
    return 0;
  }
  return 1;
});
lua_settable(luaState, -3);
    )R");
  }
}

TEST_F(LuaPluginTest, PushIndexMetaMethodFunctions) {
  auto project = Parse(R"R(
struct TestData {
  @cppFunc
  func functionReturningVoid(s32 a0, string a1) -> void;
  @cppFunc
  func functionReturningString() -> string;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("PushIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"PushIndexMetaMethod", Type{"void"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushIndexMetaMethod", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "__index");
lua_pushcfunction(luaState, [](lua_State* ls) {
  auto instance = TestData::ReadFromLua(ls, -2);
  const char* key = lua_tostring(ls, -1);
  if (0 == strcmp("functionReturningVoid", key)) {
    lua_pushcfunction(ls, [](lua_State* lsInner) {
      auto instance = TestData::ReadFromLua(lsInner, -3);
      int32_t arg0;
      LuaHelper::Read(arg0, lsInner, -2);
      std::string arg1;
      LuaHelper::Read(arg1, lsInner, -1);
      instance->functionReturningVoid(arg0, arg1);
      return 0;
    });
  } else if (0 == strcmp("functionReturningString", key)) {
    lua_pushcfunction(ls, [](lua_State* lsInner) {
      auto instance = TestData::ReadFromLua(lsInner, -1);
      auto result = instance->functionReturningString();
      LuaHelper::Push(result, lsInner);
      return 1;
    });
  } else {
    return 0;
  }
  return 1;
});
lua_settable(luaState, -3);
    )R");
  }
}

TEST_F(LuaPluginTest, PushIndexMetaMethodRefs) {
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

  ASSERT_NE(cls->GetMethod("PushIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"PushIndexMetaMethod", Type{"void"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushIndexMetaMethod", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "__index");
lua_pushcfunction(luaState, [](lua_State* ls) {
  auto instance = TestData::ReadFromLua(ls, -2);
  const char* key = lua_tostring(ls, -1);
  if (0 == strcmp("testStructWithIdRefId", key)) {
    LuaHelper::Push(instance->mTestStructWithIdRefId, ls);
  } else if (0 == strcmp("testStructNoIdRef", key)) {
    LuaHelper::Push(instance->mTestStructNoIdRef, ls);
  } else {
    return 0;
  }
  return 1;
});
lua_settable(luaState, -3);
    )R");
  }
}

TEST_F(LuaPluginTest, PushNewIndexMetaMethodFields) {
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

  ASSERT_NE(cls->GetMethod("PushNewIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"PushNewIndexMetaMethod", Type{"void"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushNewIndexMetaMethod", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "__newindex");
lua_pushcfunction(luaState, [](lua_State* ls) {
  auto instance = TestData::ReadFromLua(ls, -3);
  const char* key = lua_tostring(ls, -2);
  if (0 == strcmp("testFieldUnsigned", key)) {
    LuaHelper::Read(instance->mTestFieldUnsigned, ls, -1);
  } else if (0 == strcmp("testFieldString", key)) {
    LuaHelper::Read(instance->mTestFieldString, ls, -1);
  } else if (0 == strcmp("testFieldBool", key)) {
    LuaHelper::Read(instance->mTestFieldBool, ls, -1);
  }
  return 0;
});
lua_settable(luaState, -3);
    )R");
  }
}

TEST_F(LuaPluginTest, PushNewIndexMetaMethodRefs) {
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

  ASSERT_NE(cls->GetMethod("PushNewIndexMetaMethod", Constness::NotConst), nullptr);
  {
    auto method = ClassMethod{"PushNewIndexMetaMethod", Type{"void"}, Visibility::Private,
                              Constness::NotConst, Staticness::Static};
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    helpers::ExpectEqual(*cls->GetMethod("PushNewIndexMetaMethod", Constness::NotConst), method, R"R(
lua_pushstring(luaState, "__newindex");
lua_pushcfunction(luaState, [](lua_State* ls) {
  auto instance = TestData::ReadFromLua(ls, -3);
  const char* key = lua_tostring(ls, -2);
  if (0 == strcmp("testStructRefId", key)) {
    LuaHelper::Read(instance->mTestStructRefId, ls, -1);
  }
  return 0;
});
lua_settable(luaState, -3);
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
PushIndexMetaMethod(luaState);
PushNewIndexMetaMethod(luaState);
lua_setglobal(luaState, "TestDataMeta");
    )R");
  }
}
