// This file is generated by holgen. Do not modify manually.
#include "TestContainerEnum.h"

#include <cstring>
#include <lua.hpp>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
bool TestContainerEnum::operator==(const TestContainerEnum &rhs) const {
  return !(
      mSkills != rhs.mSkills
  );
}

const std::array<uint8_t, 4> &TestContainerEnum::GetSkills() const {
  return mSkills;
}

std::array<uint8_t, 4> &TestContainerEnum::GetSkills() {
  return mSkills;
}

void TestContainerEnum::SetSkills(const std::array<uint8_t, 4> &val) {
  mSkills = val;
}

const uint8_t &TestContainerEnum::GetSkill(TestContainerSkill idx) const {
  return mSkills[idx.GetValue()];
}

uint8_t &TestContainerEnum::GetSkill(TestContainerSkill idx) {
  return mSkills[idx.GetValue()];
}

void TestContainerEnum::SetSkill(const TestContainerSkill &idx, uint8_t val) {
  mSkills[idx.GetValue()] = val;
}

size_t TestContainerEnum::GetSkillCount() const {
  return mSkills.size();
}

bool TestContainerEnum::ParseJson(const rapidjson::Value &json, const Converter &converter) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("skills", name)) {
        auto res = JsonHelper::Parse(mSkills, data.value, converter);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerEnum.skills field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestContainerEnum: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerEnum!");
      auto res = JsonHelper::Parse(mSkills, (*it), converter);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerEnum.skills field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestContainerEnum!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestContainerEnum.");
    return false;
  }
  return true;
}

void TestContainerEnum::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerEnumMeta");
  lua_setmetatable(luaState, -2);
}

void TestContainerEnum::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "skills");
  LuaHelper::Push(mSkills, luaState, true);
  lua_settable(luaState, -3);
}

void TestContainerEnum::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerEnum *TestContainerEnum::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestContainerEnum", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerEnum *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerEnum TestContainerEnum::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerEnum{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("skills", key)) {
      LuaHelper::Read(result.mSkills, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestContainerEnum.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerEnum::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerEnum::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("skills", key)) {
    auto res = LuaHelper::Read(instance->mSkills, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerEnum.skills from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerEnum.{}", key);
  }
  return 0;
}

void TestContainerEnum::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerEnum::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerEnum::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerEnumMeta");
}

int TestContainerEnum::GetSkillCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerEnum::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerEnum.GetSkill method with an invalid lua proxy object!");
  auto arg0 = TestContainerSkill::ReadMirrorFromLua(luaState, -1);
  auto& result = instance->GetSkill(arg0);
  LuaHelper::Push(result, luaState, false);
  return 1;
}

int TestContainerEnum::GetSkillCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerEnum::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerEnum.GetSkillCount method with an invalid lua proxy object!");
  auto result = instance->GetSkillCount();
  LuaHelper::Push(result, luaState, true);
  return 1;
}

int TestContainerEnum::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("skills", key)) {
    auto instance = TestContainerEnum::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerEnum.skills with an invalid lua proxy object!");
    LuaHelper::Push(instance->mSkills, luaState, false);
  } else if (0 == strcmp("GetSkill", key)) {
    lua_pushcfunction(luaState, TestContainerEnum::GetSkillCallerFromLua);
  } else if (0 == strcmp("GetSkillCount", key)) {
    lua_pushcfunction(luaState, TestContainerEnum::GetSkillCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerEnum.{}", key);
    return 0;
  }
  return 1;
}
}
