// This file is generated by holgen. Do not modify manually.
#include "TestJsonTag.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestJsonTagManager.h"

namespace holgen_blackbox_test {
bool TestJsonTag::operator==(const TestJsonTag &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName
  );
}

uint64_t TestJsonTag::GetId() const {
  return mId;
}

const std::string &TestJsonTag::GetName() const {
  return mName;
}

std::string &TestJsonTag::GetName() {
  return mName;
}

void TestJsonTag::SetId(uint64_t val) {
  mId = val;
}

void TestJsonTag::SetName(const std::string &val) {
  mName = val;
}

TestJsonTag *TestJsonTag::Get(uint64_t id) {
  return GlobalPointer<TestJsonTagManager>::GetInstance()->GetTag(id);
}

TestJsonTag *TestJsonTag::GetFromName(const std::string &key) {
  return GlobalPointer<TestJsonTagManager>::GetInstance()->GetTagFromName(key);
}

bool TestJsonTag::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.id field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.name field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestJsonTag: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonTag!");
      auto res = JsonHelper::Parse(mId, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.id field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonTag!");
      auto res = JsonHelper::Parse(mName, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonTag.name field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestJsonTag!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestJsonTag.");
    return false;
  }
  return true;
}

void TestJsonTag::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  uint64_t id = mId;
  lua_pushstring(luaState, "i");
  lua_pushlightuserdata(luaState, reinterpret_cast<void *>(id));
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonTagMeta");
  lua_setmetatable(luaState, -2);
}

void TestJsonTag::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
}

void TestJsonTag::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonTag *TestJsonTag::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestJsonTag", className);
  lua_pushstring(luaState, "i");
  lua_gettable(luaState, idx - 1);
  uint64_t id = reinterpret_cast<uint64_t>(lua_touserdata(luaState, -1));
  auto ptr = TestJsonTag::Get(id);
  lua_pop(luaState, 1);
  return ptr;
}

TestJsonTag TestJsonTag::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestJsonTag{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonTag.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonTag::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonTag::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonTag.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonTag.name from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTag.{}", key);
  }
  return 0;
}

void TestJsonTag::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonTag::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonTag::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonTagMeta");
}

int TestJsonTag::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = TestJsonTag::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonTag.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    auto instance = TestJsonTag::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonTag.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonTag.{}", key);
    return 0;
  }
  return 1;
}
}
