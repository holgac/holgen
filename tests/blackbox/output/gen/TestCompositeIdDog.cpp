// This file is generated by holgen. Do not modify manually.
#include "TestCompositeIdDog.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
int32_t TestCompositeIdDog::GetId() const {
  return mId;
}

int16_t TestCompositeIdDog::GetVersion() const {
  return mVersion;
}

const std::string &TestCompositeIdDog::GetBreed() const {
  return mBreed;
}

std::string &TestCompositeIdDog::GetBreed() {
  return mBreed;
}

void TestCompositeIdDog::SetId(int32_t val) {
  mId = val;
}

void TestCompositeIdDog::SetVersion(int16_t val) {
  mVersion = val;
}

void TestCompositeIdDog::SetBreed(const std::string &val) {
  mBreed = val;
}

TestCompositeIdCompositeId TestCompositeIdDog::GetCompositeId() const {
  return TestCompositeIdCompositeId(TestCompositeIdObjectType::Dog, mId, mVersion);
}

bool TestCompositeIdDog::IsValid() const {
  return mId >= 0;
}

bool TestCompositeIdDog::operator==(const TestCompositeIdDog &rhs) const {
  return !(
      mId != rhs.mId ||
      mVersion != rhs.mVersion ||
      mBreed != rhs.mBreed
  );
}

bool TestCompositeIdDog::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdDog.id field");
      } else if (0 == strcmp("version", name)) {
        auto res = JsonHelper::Parse(mVersion, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdDog.version field");
      } else if (0 == strcmp("breed", name)) {
        auto res = JsonHelper::Parse(mBreed, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdDog.breed field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestCompositeIdDog: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdDog!");
      auto res = JsonHelper::Parse(mId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdDog.id field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdDog!");
      auto res = JsonHelper::Parse(mVersion, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdDog.version field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdDog!");
      auto res = JsonHelper::Parse(mBreed, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdDog.breed field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestCompositeIdDog!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestCompositeIdDog.");
    return false;
  }
  return true;
}

rapidjson::Value TestCompositeIdDog::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("id", JsonHelper::Dump(mId, doc, luaState), doc.GetAllocator());
  val.AddMember("version", JsonHelper::Dump(mVersion, doc, luaState), doc.GetAllocator());
  val.AddMember("breed", JsonHelper::Dump(mBreed, doc, luaState), doc.GetAllocator());
  return val;
}

void TestCompositeIdDog::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestCompositeIdDog");
  lua_setmetatable(luaState, -2);
}

void TestCompositeIdDog::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "version");
  LuaHelper::Push<true>(mVersion, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "breed");
  LuaHelper::Push<true>(mBreed, luaState);
  lua_settable(luaState, -3);
}

void TestCompositeIdDog::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestCompositeIdDog *TestCompositeIdDog::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestCompositeIdDog", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestCompositeIdDog *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestCompositeIdDog TestCompositeIdDog::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestCompositeIdDog{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("version", key)) {
      LuaHelper::Read(result.mVersion, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("breed", key)) {
      LuaHelper::Read(result.mBreed, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestCompositeIdDog.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestCompositeIdDog::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdDog.id from lua failed!");
  } else if (0 == strcmp("version", key)) {
    auto res = LuaHelper::Read(instance->mVersion, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdDog.version from lua failed!");
  } else if (0 == strcmp("breed", key)) {
    auto res = LuaHelper::Read(instance->mBreed, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdDog.breed from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestCompositeIdDog.{}", key);
  }
  return 0;
}

int TestCompositeIdDog::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdDog.operator== method with an invalid lua proxy object!");
  TestCompositeIdDog arg0Mirror;
  TestCompositeIdDog *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestCompositeIdDog::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestCompositeIdDog::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestCompositeIdDog::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestCompositeIdDog::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestCompositeIdDog::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestCompositeIdDog::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestCompositeIdDog");
}

int TestCompositeIdDog::GetCompositeIdCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdDog.GetCompositeId method with an invalid lua proxy object!");
  auto result = instance->GetCompositeId();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestCompositeIdDog::IsValidCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdDog.IsValid method with an invalid lua proxy object!");
  auto result = instance->IsValid();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestCompositeIdDog::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdDog.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("version", key)) {
    auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdDog.version with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mVersion, luaState);
  } else if (0 == strcmp("breed", key)) {
    auto instance = TestCompositeIdDog::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdDog.breed with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mBreed, luaState);
  } else if (0 == strcmp("GetCompositeId", key)) {
    lua_pushcfunction(luaState, TestCompositeIdDog::GetCompositeIdCallerFromLua);
  } else if (0 == strcmp("IsValid", key)) {
    lua_pushcfunction(luaState, TestCompositeIdDog::IsValidCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestCompositeIdDog.{}", key);
    return 0;
  }
  return 1;
}
}
