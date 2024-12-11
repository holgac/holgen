// This file is generated by holgen. Do not modify manually.
#include "TestJsonStructContainer.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<TestJsonTag> &TestJsonStructContainer::GetTags() const {
  return mTags;
}

std::vector<TestJsonTag> &TestJsonStructContainer::GetTags() {
  return mTags;
}

const std::vector<TestJsonEnum> &TestJsonStructContainer::GetEnums() const {
  return mEnums;
}

std::vector<TestJsonEnum> &TestJsonStructContainer::GetEnums() {
  return mEnums;
}

const std::map<std::string, TestJsonTag> &TestJsonStructContainer::GetTagMap() const {
  return mTagMap;
}

std::map<std::string, TestJsonTag> &TestJsonStructContainer::GetTagMap() {
  return mTagMap;
}

void TestJsonStructContainer::SetTags(const std::vector<TestJsonTag> &val) {
  mTags = val;
}

void TestJsonStructContainer::SetEnums(const std::vector<TestJsonEnum> &val) {
  mEnums = val;
}

void TestJsonStructContainer::SetTagMap(const std::map<std::string, TestJsonTag> &val) {
  mTagMap = val;
}

bool TestJsonStructContainer::operator==(const TestJsonStructContainer &rhs) const {
  return !(
      mTags != rhs.mTags ||
      mEnums != rhs.mEnums ||
      mTagMap != rhs.mTagMap
  );
}

bool TestJsonStructContainer::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("tags", name)) {
        auto res = JsonHelper::Parse(mTags, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructContainer.tags field");
      } else if (0 == strcmp("enums", name)) {
        auto res = JsonHelper::Parse(mEnums, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructContainer.enums field");
      } else if (0 == strcmp("tagMap", name)) {
        auto res = JsonHelper::Parse(mTagMap, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructContainer.tagMap field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestJsonStructContainer: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonStructContainer!");
      auto res = JsonHelper::Parse(mTags, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructContainer.tags field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonStructContainer!");
      auto res = JsonHelper::Parse(mEnums, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructContainer.enums field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestJsonStructContainer!");
      auto res = JsonHelper::Parse(mTagMap, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestJsonStructContainer.tagMap field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestJsonStructContainer!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestJsonStructContainer.");
    return false;
  }
  return true;
}

rapidjson::Value TestJsonStructContainer::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("tags", JsonHelper::Dump(mTags, doc, luaState), doc.GetAllocator());
  val.AddMember("enums", JsonHelper::Dump(mEnums, doc, luaState), doc.GetAllocator());
  val.AddMember("tagMap", JsonHelper::Dump(mTagMap, doc, luaState), doc.GetAllocator());
  return val;
}

void TestJsonStructContainer::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestJsonStructContainer");
  lua_setmetatable(luaState, -2);
}

void TestJsonStructContainer::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "tags");
  LuaHelper::Push<true>(mTags, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "enums");
  LuaHelper::Push<true>(mEnums, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "tagMap");
  LuaHelper::Push<true>(mTagMap, luaState);
  lua_settable(luaState, -3);
}

void TestJsonStructContainer::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestJsonStructContainer *TestJsonStructContainer::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestJsonStructContainer", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestJsonStructContainer *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestJsonStructContainer TestJsonStructContainer::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestJsonStructContainer{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("tags", key)) {
      LuaHelper::Read(result.mTags, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("enums", key)) {
      LuaHelper::Read(result.mEnums, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("tagMap", key)) {
      LuaHelper::Read(result.mTagMap, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestJsonStructContainer.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestJsonStructContainer::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestJsonStructContainer::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("tags", key)) {
    auto res = LuaHelper::Read(instance->mTags, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonStructContainer.tags from lua failed!");
  } else if (0 == strcmp("enums", key)) {
    auto res = LuaHelper::Read(instance->mEnums, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonStructContainer.enums from lua failed!");
  } else if (0 == strcmp("tagMap", key)) {
    auto res = LuaHelper::Read(instance->mTagMap, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestJsonStructContainer.tagMap from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructContainer.{}", key);
  }
  return 0;
}

int TestJsonStructContainer::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestJsonStructContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestJsonStructContainer.operator== method with an invalid lua proxy object!");
  TestJsonStructContainer arg0Mirror;
  TestJsonStructContainer *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestJsonStructContainer::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestJsonStructContainer::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestJsonStructContainer::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestJsonStructContainer::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestJsonStructContainer::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestJsonStructContainer::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestJsonStructContainer");
}

int TestJsonStructContainer::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("tags", key)) {
    auto instance = TestJsonStructContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructContainer.tags with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mTags, luaState);
  } else if (0 == strcmp("enums", key)) {
    auto instance = TestJsonStructContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructContainer.enums with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mEnums, luaState);
  } else if (0 == strcmp("tagMap", key)) {
    auto instance = TestJsonStructContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestJsonStructContainer.tagMap with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mTagMap, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestJsonStructContainer.{}", key);
    return 0;
  }
  return 1;
}
}
