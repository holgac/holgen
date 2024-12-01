// This file is generated by holgen. Do not modify manually.
#include "TestContainerMap.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::map<uint32_t, TestContainerInnerStructWithId> &TestContainerMap::GetInnerStructsWithId() const {
  return mInnerStructsWithId;
}

std::map<uint32_t, TestContainerInnerStructWithId> &TestContainerMap::GetInnerStructsWithId() {
  return mInnerStructsWithId;
}

void TestContainerMap::SetInnerStructsWithId(const std::map<uint32_t, TestContainerInnerStructWithId> &val) {
  mInnerStructsWithId = val;
}

const TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithIdFromName(const std::string &key) const {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId.at(it->second);
}

TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithIdFromName(const std::string &key) {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId.at(it->second);
}

TestContainerInnerStructWithId *TestContainerMap::AddInnerStructWithId(TestContainerInnerStructWithId &&elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mInnerStructsWithIdNextId;
  ++mInnerStructsWithIdNextId;
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != TestContainerInnerStructWithId::IdType(-1) && idInElem != TestContainerInnerStructWithId::IdType(newId), "Objects not loaded in the right order!");
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  auto[it, res] = mInnerStructsWithId.emplace(newId, std::move(elem));
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Corrupt internal ID counter - was TestContainerMap.innerStructsWithId modified externally?");
  auto& newElem = it->second;
  newElem.SetId(newId);
  return &newElem;
}

TestContainerInnerStructWithId *TestContainerMap::AddInnerStructWithId(TestContainerInnerStructWithId &elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mInnerStructsWithIdNextId;
  ++mInnerStructsWithIdNextId;
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != TestContainerInnerStructWithId::IdType(-1) && idInElem != TestContainerInnerStructWithId::IdType(newId), "Objects not loaded in the right order!");
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  auto[it, res] = mInnerStructsWithId.emplace(newId, elem);
  HOLGEN_WARN_AND_RETURN_IF(!res, nullptr, "Corrupt internal ID counter - was TestContainerMap.innerStructsWithId modified externally?");
  auto& newElem = it->second;
  newElem.SetId(newId);
  return &newElem;
}

const TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithId(uint32_t idx) const {
  auto it = mInnerStructsWithId.find(idx);
  if (it == mInnerStructsWithId.end())
    return nullptr;
  return &it->second;
}

TestContainerInnerStructWithId *TestContainerMap::GetInnerStructWithId(uint32_t idx) {
  auto it = mInnerStructsWithId.find(idx);
  if (it == mInnerStructsWithId.end())
    return nullptr;
  return &it->second;
}

void TestContainerMap::DeleteInnerStructWithId(uint32_t key) {
  auto ptr = GetInnerStructWithId(key);
  mInnerStructsWithIdNameIndex.erase(ptr->GetName());
  mInnerStructsWithId.erase(key);
}

bool TestContainerMap::HasInnerStructWithId(uint32_t key) const {
  return mInnerStructsWithId.contains(key);
}

size_t TestContainerMap::GetInnerStructWithIdCount() const {
  return mInnerStructsWithId.size();
}

bool TestContainerMap::operator==(const TestContainerMap &rhs) const {
  return !(
      mInnerStructsWithId != rhs.mInnerStructsWithId ||
      mInnerStructsWithIdNameIndex != rhs.mInnerStructsWithIdNameIndex ||
      mInnerStructsWithIdNextId != rhs.mInnerStructsWithIdNextId
  );
}

bool TestContainerMap::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("innerStructsWithId", name)) {
        auto res = JsonHelper::Parse(mInnerStructsWithId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerMap.innerStructsWithId field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestContainerMap: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerMap!");
      auto res = JsonHelper::Parse(mInnerStructsWithId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerMap.innerStructsWithId field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestContainerMap!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestContainerMap.");
    return false;
  }
  return true;
}

rapidjson::Value TestContainerMap::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("innerStructsWithId", JsonHelper::Dump(mInnerStructsWithId, doc, luaState), doc.GetAllocator());
  return val;
}

void TestContainerMap::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerMap");
  lua_setmetatable(luaState, -2);
}

void TestContainerMap::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "innerStructsWithId");
  LuaHelper::Push<true>(mInnerStructsWithId, luaState);
  lua_settable(luaState, -3);
}

void TestContainerMap::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerMap *TestContainerMap::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestContainerMap", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerMap *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerMap TestContainerMap::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerMap{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Read(result.mInnerStructsWithId, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestContainerMap.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerMap::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("innerStructsWithId", key)) {
    auto res = LuaHelper::Read(instance->mInnerStructsWithId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerMap.innerStructsWithId from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerMap.{}", key);
  }
  return 0;
}

void TestContainerMap::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerMap::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerMap::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerMap");
}

int TestContainerMap::GetInnerStructWithIdFromNameCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerMap.GetInnerStructWithIdFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetInnerStructWithIdFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerMap::AddInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerMap.AddInnerStructWithId method with an invalid lua proxy object!");
  auto arg0 = TestContainerInnerStructWithId::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddInnerStructWithId(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerMap::GetInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerMap.GetInnerStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetInnerStructWithId(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerMap::DeleteInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerMap.DeleteInnerStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteInnerStructWithId(arg0);
  return 0;
}

int TestContainerMap::HasInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerMap.HasInnerStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->HasInnerStructWithId(arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestContainerMap::GetInnerStructWithIdCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerMap::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerMap.GetInnerStructWithIdCount method with an invalid lua proxy object!");
  auto result = instance->GetInnerStructWithIdCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestContainerMap::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("innerStructsWithId", key)) {
    auto instance = TestContainerMap::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerMap.innerStructsWithId with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mInnerStructsWithId, luaState);
  } else if (0 == strcmp("GetInnerStructWithIdFromName", key)) {
    lua_pushcfunction(luaState, TestContainerMap::GetInnerStructWithIdFromNameCallerFromLua);
  } else if (0 == strcmp("AddInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerMap::AddInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("GetInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerMap::GetInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("DeleteInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerMap::DeleteInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("HasInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerMap::HasInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("GetInnerStructWithIdCount", key)) {
    lua_pushcfunction(luaState, TestContainerMap::GetInnerStructWithIdCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerMap.{}", key);
    return 0;
  }
  return 1;
}
}
