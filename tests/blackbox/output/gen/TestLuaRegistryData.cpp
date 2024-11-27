// This file is generated by holgen. Do not modify manually.
#include "TestLuaRegistryData.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
TestLuaRegistryData::TestLuaRegistryData(TestLuaRegistryData &&rhs) noexcept {
  std::swap(mData, rhs.mData);
  mTable = std::move(rhs.mTable);
}

TestLuaRegistryData::~TestLuaRegistryData() {
  HOLGEN_WARN_IF(mData != LUA_NOREF, "TestLuaRegistryData.data was not released!");
}

void TestLuaRegistryData::InitializeLua(lua_State *luaState) {
  HOLGEN_WARN_IF(mData != LUA_NOREF, "TestLuaRegistryData.data was already initialized!");
  lua_newtable(luaState);
  mData = luaL_ref(luaState, LUA_REGISTRYINDEX);
}

void TestLuaRegistryData::UninitializeLua(lua_State *luaState) {
  luaL_unref(luaState, LUA_REGISTRYINDEX, mData);
  mData = LUA_NOREF;
}

int TestLuaRegistryData::GetData() const {
  return mData;
}

void TestLuaRegistryData::SetData(int val) {
  mData = val;
}

void TestLuaRegistryData::SetTable(std::string val) {
  mTable = std::move(val);
}

const std::string &TestLuaRegistryData::GetTable() const {
  return mTable;
}

void TestLuaRegistryData::Init(lua_State *luaState, const std::function<void(lua_State *)> &initData) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset Init function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling Init", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "Init");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Init function in {}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_copy(luaState, -1, -2);
  lua_pop(luaState, 1);
  LuaHelper::Push<false>(*this, luaState);
  initData(luaState);
  lua_call(luaState, 2, 0);
}

int32_t TestLuaRegistryData::Get(lua_State *luaState) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), {}, "Calling unset Get function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling Get", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_pushstring(luaState, "Get");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Get function in {}", mTable);
    lua_pop(luaState, 1);
    return {};
  }
  lua_copy(luaState, -1, -2);
  lua_pop(luaState, 1);
  LuaHelper::Push<false>(*this, luaState);
  lua_call(luaState, 1, 1);
  int32_t result;
  LuaHelper::Read(result, luaState, -1);
  lua_pop(luaState, 1);
  return result;
}

void TestLuaRegistryData::Add(lua_State *luaState, const int32_t val) const {
  HOLGEN_WARN_AND_RETURN_IF(mTable.empty(), void(), "Calling unset Add function from table");
  lua_getglobal(luaState, mTable.c_str());
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Function table {} not found when calling Add", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_pushstring(luaState, "Add");
  lua_gettable(luaState, -2);
  if (lua_isnil(luaState, -1)) {
    HOLGEN_WARN("Calling undefined Add function in {}", mTable);
    lua_pop(luaState, 1);
    return void();
  }
  lua_copy(luaState, -1, -2);
  lua_pop(luaState, 1);
  LuaHelper::Push<false>(*this, luaState);
  LuaHelper::Push<false>(val, luaState);
  lua_call(luaState, 2, 0);
}

bool TestLuaRegistryData::operator==(const TestLuaRegistryData &rhs) const {
  return !(
      mData != rhs.mData ||
      mTable != rhs.mTable
  );
}

bool TestLuaRegistryData::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("table", name)) {
        auto res = JsonHelper::Parse(mTable, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaRegistryData.mTable field");
      } else if (0 == strcmp("data", name)) {
        mData = JsonHelper::ParseLuaRegistryObject(data.value, luaState);
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestLuaRegistryData: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaRegistryData!");
      auto res = JsonHelper::Parse(mTable, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestLuaRegistryData.mTable field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestLuaRegistryData!");
      mData = JsonHelper::ParseLuaRegistryObject((*it), luaState);
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestLuaRegistryData!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestLuaRegistryData.");
    return false;
  }
  return true;
}

rapidjson::Value TestLuaRegistryData::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("table", JsonHelper::Dump(mTable, doc, luaState), doc.GetAllocator());
  val.AddMember("data", JsonHelper::DumpLuaRegistryObject(mData, doc, luaState), doc.GetAllocator());
  return val;
}

void TestLuaRegistryData::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestLuaRegistryData");
  lua_setmetatable(luaState, -2);
}

void TestLuaRegistryData::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestLuaRegistryData *TestLuaRegistryData::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestLuaRegistryData", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestLuaRegistryData *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

int TestLuaRegistryData::NewIndexMetaMethod(lua_State *luaState) {
  return 0;
}

void TestLuaRegistryData::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestLuaRegistryData::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestLuaRegistryData::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestLuaRegistryData");
}

int TestLuaRegistryData::InitCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaRegistryData.Init method with an invalid lua proxy object!");
  std::function<void(lua_State *)> arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->Init(luaState, arg0);
  return 0;
}

int TestLuaRegistryData::GetCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaRegistryData.Get method with an invalid lua proxy object!");
  auto result = instance->Get(luaState);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestLuaRegistryData::AddCallerFromLua(lua_State *luaState) {
  auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestLuaRegistryData.Add method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->Add(luaState, arg0);
  return 0;
}

int TestLuaRegistryData::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("data", key)) {
    auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestLuaRegistryData.data with an invalid lua proxy object!");
    lua_rawgeti(luaState, LUA_REGISTRYINDEX, instance->mData);
  } else if (0 == strcmp("Init", key)) {
    lua_pushcfunction(luaState, TestLuaRegistryData::InitCallerFromLua);
  } else if (0 == strcmp("Get", key)) {
    lua_pushcfunction(luaState, TestLuaRegistryData::GetCallerFromLua);
  } else if (0 == strcmp("Add", key)) {
    lua_pushcfunction(luaState, TestLuaRegistryData::AddCallerFromLua);
  } else if (0 == strcmp("TABLE", key)) {
    auto instance = TestLuaRegistryData::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push<false>(instance->mTable, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestLuaRegistryData.{}", key);
    return 0;
  }
  return 1;
}

TestLuaRegistryData &TestLuaRegistryData::operator=(TestLuaRegistryData &&rhs) noexcept {
  std::swap(mData, rhs.mData);
  mTable = std::move(rhs.mTable);
  return *this;
}
}
