// This file is generated by holgen. Do not modify manually.
#include "TestCompositeIdCat.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
int32_t TestCompositeIdCat::GetId() const {
  return mId;
}

int16_t TestCompositeIdCat::GetVersion() const {
  return mVersion;
}

const std::string &TestCompositeIdCat::GetName() const {
  return mName;
}

std::string &TestCompositeIdCat::GetName() {
  return mName;
}

const std::string &TestCompositeIdCat::GetBreed() const {
  return mBreed;
}

std::string &TestCompositeIdCat::GetBreed() {
  return mBreed;
}

void TestCompositeIdCat::SetId(int32_t val) {
  mId = val;
}

void TestCompositeIdCat::SetVersion(int16_t val) {
  mVersion = val;
}

void TestCompositeIdCat::SetName(const std::string &val) {
  mName = val;
}

void TestCompositeIdCat::SetBreed(const std::string &val) {
  mBreed = val;
}

TestCompositeIdCompositeId TestCompositeIdCat::GetCompositeId() const {
  return TestCompositeIdCompositeId(TestCompositeIdObjectType::Cat, mId, mVersion);
}

bool TestCompositeIdCat::IsValid() const {
  return mId >= 0;
}

bool TestCompositeIdCat::operator==(const TestCompositeIdCat &rhs) const {
  return !(
      mId != rhs.mId ||
      mVersion != rhs.mVersion ||
      mName != rhs.mName ||
      mBreed != rhs.mBreed
  );
}

bool TestCompositeIdCat::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("id", name)) {
        auto res = JsonHelper::Parse(mId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.id field");
      } else if (0 == strcmp("version", name)) {
        auto res = JsonHelper::Parse(mVersion, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.version field");
      } else if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.name field");
      } else if (0 == strcmp("breed", name)) {
        auto res = JsonHelper::Parse(mBreed, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.breed field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestCompositeIdCat: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdCat!");
      auto res = JsonHelper::Parse(mId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.id field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdCat!");
      auto res = JsonHelper::Parse(mVersion, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.version field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdCat!");
      auto res = JsonHelper::Parse(mName, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdCat!");
      auto res = JsonHelper::Parse(mBreed, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdCat.breed field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestCompositeIdCat!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestCompositeIdCat.");
    return false;
  }
  return true;
}

rapidjson::Value TestCompositeIdCat::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("id", JsonHelper::Dump(mId, doc, luaState), doc.GetAllocator());
  val.AddMember("version", JsonHelper::Dump(mVersion, doc, luaState), doc.GetAllocator());
  val.AddMember("name", JsonHelper::Dump(mName, doc, luaState), doc.GetAllocator());
  val.AddMember("breed", JsonHelper::Dump(mBreed, doc, luaState), doc.GetAllocator());
  return val;
}

void TestCompositeIdCat::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestCompositeIdCat");
  lua_setmetatable(luaState, -2);
}

void TestCompositeIdCat::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "version");
  LuaHelper::Push<true>(mVersion, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "breed");
  LuaHelper::Push<true>(mBreed, luaState);
  lua_settable(luaState, -3);
}

void TestCompositeIdCat::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestCompositeIdCat *TestCompositeIdCat::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestCompositeIdCat", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestCompositeIdCat *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestCompositeIdCat TestCompositeIdCat::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestCompositeIdCat{};
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
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("breed", key)) {
      LuaHelper::Read(result.mBreed, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestCompositeIdCat.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestCompositeIdCat::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdCat.id from lua failed!");
  } else if (0 == strcmp("version", key)) {
    auto res = LuaHelper::Read(instance->mVersion, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdCat.version from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdCat.name from lua failed!");
  } else if (0 == strcmp("breed", key)) {
    auto res = LuaHelper::Read(instance->mBreed, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdCat.breed from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestCompositeIdCat.{}", key);
  }
  return 0;
}

void TestCompositeIdCat::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestCompositeIdCat::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestCompositeIdCat::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestCompositeIdCat");
}

int TestCompositeIdCat::GetCompositeIdCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdCat.GetCompositeId method with an invalid lua proxy object!");
  auto result = instance->GetCompositeId();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestCompositeIdCat::IsValidCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdCat.IsValid method with an invalid lua proxy object!");
  auto result = instance->IsValid();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestCompositeIdCat::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdCat.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("version", key)) {
    auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdCat.version with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mVersion, luaState);
  } else if (0 == strcmp("name", key)) {
    auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdCat.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else if (0 == strcmp("breed", key)) {
    auto instance = TestCompositeIdCat::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdCat.breed with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mBreed, luaState);
  } else if (0 == strcmp("GetCompositeId", key)) {
    lua_pushcfunction(luaState, TestCompositeIdCat::GetCompositeIdCallerFromLua);
  } else if (0 == strcmp("IsValid", key)) {
    lua_pushcfunction(luaState, TestCompositeIdCat::IsValidCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestCompositeIdCat.{}", key);
    return 0;
  }
  return 1;
}
}
