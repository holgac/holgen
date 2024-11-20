// This file is generated by holgen. Do not modify manually.
#include "TestVariantStructCat.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::string &TestVariantStructCat::GetName() const {
  return mName;
}

std::string &TestVariantStructCat::GetName() {
  return mName;
}

const std::string &TestVariantStructCat::GetColor() const {
  return mColor;
}

std::string &TestVariantStructCat::GetColor() {
  return mColor;
}

void TestVariantStructCat::SetName(const std::string &val) {
  mName = val;
}

void TestVariantStructCat::SetColor(const std::string &val) {
  mColor = val;
}

bool TestVariantStructCat::operator==(const TestVariantStructCat &rhs) const {
  return !(
      mName != rhs.mName ||
      mColor != rhs.mColor
  );
}

bool TestVariantStructCat::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructCat.name field");
      } else if (0 == strcmp("color", name)) {
        auto res = JsonHelper::Parse(mColor, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructCat.color field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestVariantStructCat: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructCat!");
      auto res = JsonHelper::Parse(mName, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructCat.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestVariantStructCat!");
      auto res = JsonHelper::Parse(mColor, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestVariantStructCat.color field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestVariantStructCat!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestVariantStructCat.");
    return false;
  }
  return true;
}

rapidjson::Value TestVariantStructCat::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("name", JsonHelper::Dump(mName, doc, luaState), doc.GetAllocator());
  val.AddMember("color", JsonHelper::Dump(mColor, doc, luaState), doc.GetAllocator());
  return val;
}

void TestVariantStructCat::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestVariantStructCat");
  lua_setmetatable(luaState, -2);
}

void TestVariantStructCat::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "color");
  LuaHelper::Push<true>(mColor, luaState);
  lua_settable(luaState, -3);
}

void TestVariantStructCat::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestVariantStructCat *TestVariantStructCat::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestVariantStructCat", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestVariantStructCat *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestVariantStructCat TestVariantStructCat::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestVariantStructCat{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("color", key)) {
      LuaHelper::Read(result.mColor, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestVariantStructCat.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestVariantStructCat::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestVariantStructCat::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructCat.name from lua failed!");
  } else if (0 == strcmp("color", key)) {
    auto res = LuaHelper::Read(instance->mColor, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestVariantStructCat.color from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructCat.{}", key);
  }
  return 0;
}

void TestVariantStructCat::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestVariantStructCat::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestVariantStructCat::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestVariantStructCat");
}

int TestVariantStructCat::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("name", key)) {
    auto instance = TestVariantStructCat::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructCat.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else if (0 == strcmp("color", key)) {
    auto instance = TestVariantStructCat::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestVariantStructCat.color with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mColor, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestVariantStructCat.{}", key);
    return 0;
  }
  return 1;
}
}
