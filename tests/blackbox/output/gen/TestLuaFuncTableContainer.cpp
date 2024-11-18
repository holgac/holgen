// This file is generated by holgen. Do not modify manually.
#include "TestLuaFuncTableContainer.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
int32_t TestLuaFuncTableContainer::GetField() const {
  return mField;
}

const TestLuaFuncTable &TestLuaFuncTableContainer::GetScript1() const {
  return mScript1;
}

TestLuaFuncTable &TestLuaFuncTableContainer::GetScript1() {
  return mScript1;
}

const TestLuaFuncTable &TestLuaFuncTableContainer::GetScript2() const {
  return mScript2;
}

TestLuaFuncTable &TestLuaFuncTableContainer::GetScript2() {
  return mScript2;
}

const TestLuaFuncTableWithSourceTable &TestLuaFuncTableContainer::GetScriptWithSourceTable1() const {
  return mScriptWithSourceTable1;
}

TestLuaFuncTableWithSourceTable &TestLuaFuncTableContainer::GetScriptWithSourceTable1() {
  return mScriptWithSourceTable1;
}

const TestLuaFuncTableWithSourceTable &TestLuaFuncTableContainer::GetScriptWithSourceTable2() const {
  return mScriptWithSourceTable2;
}

TestLuaFuncTableWithSourceTable &TestLuaFuncTableContainer::GetScriptWithSourceTable2() {
  return mScriptWithSourceTable2;
}

const TestLuaFuncTableStatic &TestLuaFuncTableContainer::GetStaticScript1() const {
  return mStaticScript1;
}

TestLuaFuncTableStatic &TestLuaFuncTableContainer::GetStaticScript1() {
  return mStaticScript1;
}

const TestLuaFuncTableStatic &TestLuaFuncTableContainer::GetStaticScript2() const {
  return mStaticScript2;
}

TestLuaFuncTableStatic &TestLuaFuncTableContainer::GetStaticScript2() {
  return mStaticScript2;
}

void TestLuaFuncTableContainer::SetField(int32_t val) {
  mField = val;
}

void TestLuaFuncTableContainer::SetScript1(const TestLuaFuncTable &val) {
  mScript1 = val;
}

void TestLuaFuncTableContainer::SetScript2(const TestLuaFuncTable &val) {
  mScript2 = val;
}

void TestLuaFuncTableContainer::SetScriptWithSourceTable1(const TestLuaFuncTableWithSourceTable &val) {
  mScriptWithSourceTable1 = val;
}

void TestLuaFuncTableContainer::SetScriptWithSourceTable2(const TestLuaFuncTableWithSourceTable &val) {
  mScriptWithSourceTable2 = val;
}

void TestLuaFuncTableContainer::SetStaticScript1(const TestLuaFuncTableStatic &val) {
  mStaticScript1 = val;
}

void TestLuaFuncTableContainer::SetStaticScript2(const TestLuaFuncTableStatic &val) {
  mStaticScript2 = val;
}

bool TestLuaFuncTableContainer::operator==(const TestLuaFuncTableContainer &rhs) const {
  return !(
      mField != rhs.mField ||
      mScript1 != rhs.mScript1 ||
      mScript2 != rhs.mScript2 ||
      mScriptWithSourceTable1 != rhs.mScriptWithSourceTable1 ||
      mScriptWithSourceTable2 != rhs.mScriptWithSourceTable2 ||
      mStaticScript1 != rhs.mStaticScript1 ||
      mStaticScript2 != rhs.mStaticScript2
  );
}

bool TestLuaFuncTableContainer::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("field", name)) {
        auto res = JsonHelper::Parse(mField, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.field field");
      } else if (0 == strcmp("script1", name)) {
        auto res = JsonHelper::Parse(mScript1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.script1 field");
      } else if (0 == strcmp("script2", name)) {
        auto res = JsonHelper::Parse(mScript2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.script2 field");
      } else if (0 == strcmp("scriptWithSourceTable1", name)) {
        auto res = JsonHelper::Parse(mScriptWithSourceTable1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.scriptWithSourceTable1 field");
      } else if (0 == strcmp("scriptWithSourceTable2", name)) {
        auto res = JsonHelper::Parse(mScriptWithSourceTable2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.scriptWithSourceTable2 field");
      } else if (0 == strcmp("staticScript1", name)) {
        auto res = JsonHelper::Parse(mStaticScript1, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.staticScript1 field");
      } else if (0 == strcmp("staticScript2", name)) {
        auto res = JsonHelper::Parse(mStaticScript2, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.staticScript2 field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestLuaFuncTableContainer: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mField, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.field field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mScript1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.script1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mScript2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.script2 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mScriptWithSourceTable1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.scriptWithSourceTable1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mScriptWithSourceTable2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.scriptWithSourceTable2 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mStaticScript1, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.staticScript1 field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaFuncTableContainer!");
      auto res = JsonHelper::Parse(mStaticScript2, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaFuncTableContainer.staticScript2 field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestLuaFuncTableContainer!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestLuaFuncTableContainer.");
    return false;
  }
  return true;
}

rapidjson::Value TestLuaFuncTableContainer::DumpJson(rapidjson::Document &doc) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("field", JsonHelper::Dump(mField, doc), doc.GetAllocator());
  val.AddMember("script1", JsonHelper::Dump(mScript1, doc), doc.GetAllocator());
  val.AddMember("script2", JsonHelper::Dump(mScript2, doc), doc.GetAllocator());
  val.AddMember("scriptWithSourceTable1", JsonHelper::Dump(mScriptWithSourceTable1, doc), doc.GetAllocator());
  val.AddMember("scriptWithSourceTable2", JsonHelper::Dump(mScriptWithSourceTable2, doc), doc.GetAllocator());
  val.AddMember("staticScript1", JsonHelper::Dump(mStaticScript1, doc), doc.GetAllocator());
  val.AddMember("staticScript2", JsonHelper::Dump(mStaticScript2, doc), doc.GetAllocator());
  return val;
}

void TestLuaFuncTableContainer::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaFuncTableContainer");
  lua_setmetatable(luaState, -2);
}

void TestLuaFuncTableContainer::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "field");
  LuaHelper::Push<true>(mField, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "script1");
  mScript1.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "script2");
  mScript2.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "scriptWithSourceTable1");
  mScriptWithSourceTable1.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "scriptWithSourceTable2");
  mScriptWithSourceTable2.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "staticScript1");
  mStaticScript1.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "staticScript2");
  mStaticScript2.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
}

void TestLuaFuncTableContainer::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaFuncTableContainer *TestLuaFuncTableContainer::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestLuaFuncTableContainer", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaFuncTableContainer *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestLuaFuncTableContainer TestLuaFuncTableContainer::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestLuaFuncTableContainer{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("field", key)) {
      LuaHelper::Read(result.mField, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("script1", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mScript1 = *TestLuaFuncTable::ReadProxyFromLua(luaState, -1);
      } else {
        result.mScript1 = TestLuaFuncTable::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("script2", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mScript2 = *TestLuaFuncTable::ReadProxyFromLua(luaState, -1);
      } else {
        result.mScript2 = TestLuaFuncTable::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("scriptWithSourceTable1", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mScriptWithSourceTable1 = *TestLuaFuncTableWithSourceTable::ReadProxyFromLua(luaState, -1);
      } else {
        result.mScriptWithSourceTable1 = TestLuaFuncTableWithSourceTable::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("scriptWithSourceTable2", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mScriptWithSourceTable2 = *TestLuaFuncTableWithSourceTable::ReadProxyFromLua(luaState, -1);
      } else {
        result.mScriptWithSourceTable2 = TestLuaFuncTableWithSourceTable::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("staticScript1", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mStaticScript1 = *TestLuaFuncTableStatic::ReadProxyFromLua(luaState, -1);
      } else {
        result.mStaticScript1 = TestLuaFuncTableStatic::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else if (0 == strcmp("staticScript2", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mStaticScript2 = *TestLuaFuncTableStatic::ReadProxyFromLua(luaState, -1);
      } else {
        result.mStaticScript2 = TestLuaFuncTableStatic::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestLuaFuncTableContainer.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestLuaFuncTableContainer::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("field", key)) {
    auto res = LuaHelper::Read(instance->mField, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.field from lua failed!");
  } else if (0 == strcmp("script1", key)) {
    auto res = LuaHelper::Read(instance->mScript1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.script1 from lua failed!");
  } else if (0 == strcmp("script2", key)) {
    auto res = LuaHelper::Read(instance->mScript2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.script2 from lua failed!");
  } else if (0 == strcmp("scriptWithSourceTable1", key)) {
    auto res = LuaHelper::Read(instance->mScriptWithSourceTable1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.scriptWithSourceTable1 from lua failed!");
  } else if (0 == strcmp("scriptWithSourceTable2", key)) {
    auto res = LuaHelper::Read(instance->mScriptWithSourceTable2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.scriptWithSourceTable2 from lua failed!");
  } else if (0 == strcmp("staticScript1", key)) {
    auto res = LuaHelper::Read(instance->mStaticScript1, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.staticScript1 from lua failed!");
  } else if (0 == strcmp("staticScript2", key)) {
    auto res = LuaHelper::Read(instance->mStaticScript2, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestLuaFuncTableContainer.staticScript2 from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaFuncTableContainer.{}", key);
  }
  return 0;
}

void TestLuaFuncTableContainer::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaFuncTableContainer::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaFuncTableContainer::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaFuncTableContainer");
}

int TestLuaFuncTableContainer::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("field", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.field with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mField, luaState);
  } else if (0 == strcmp("script1", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.script1 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mScript1, luaState);
  } else if (0 == strcmp("script2", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.script2 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mScript2, luaState);
  } else if (0 == strcmp("scriptWithSourceTable1", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.scriptWithSourceTable1 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mScriptWithSourceTable1, luaState);
  } else if (0 == strcmp("scriptWithSourceTable2", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.scriptWithSourceTable2 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mScriptWithSourceTable2, luaState);
  } else if (0 == strcmp("staticScript1", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.staticScript1 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mStaticScript1, luaState);
  } else if (0 == strcmp("staticScript2", key)) {
    auto instance = TestLuaFuncTableContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaFuncTableContainer.staticScript2 with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mStaticScript2, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaFuncTableContainer.{}", key);
    return 0;
  }
  return 1;
}
}
