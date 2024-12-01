// This file is generated by holgen. Do not modify manually.
#include "TestCompositeIdContainer.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "TestCompositeIdCompositeId.h"

namespace holgen_blackbox_test {
const std::deque<TestCompositeIdHuman> &TestCompositeIdContainer::GetHumans() const {
  return mHumans;
}

std::deque<TestCompositeIdHuman> &TestCompositeIdContainer::GetHumans() {
  return mHumans;
}

void TestCompositeIdContainer::SetHumans(const std::deque<TestCompositeIdHuman> &val) {
  mHumans = val;
}

const TestCompositeIdHuman *TestCompositeIdContainer::GetHumanFromName(const std::string &key) const {
  auto it = mHumansNameIndex.find(key);
  if (it == mHumansNameIndex.end())
    return nullptr;
  return &mHumans[it->second];
}

TestCompositeIdHuman *TestCompositeIdContainer::GetHumanFromName(const std::string &key) {
  auto it = mHumansNameIndex.find(key);
  if (it == mHumansNameIndex.end())
    return nullptr;
  return &mHumans[it->second];
}

TestCompositeIdHuman *TestCompositeIdContainer::AddHuman(TestCompositeIdHuman &&elem) {
  if (mHumansNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  if (mHumansNextDeletedIndex > 0) {
    auto newId = mHumansNextDeletedIndex - 1;
    mHumansNextDeletedIndex = -1 - mHumans[newId].GetId();
    auto idInElem = elem.GetId();
    HOLGEN_FAIL_IF(idInElem != TestCompositeIdHuman::IdType(-1) && idInElem != TestCompositeIdHuman::IdType(newId), "Objects not loaded in the right order!");
    elem.SetId(newId);
    mHumansNameIndex.emplace(elem.GetName(), newId);
    elem.SetVersion(mHumans[newId].GetVersion());
    mHumans[newId] = std::move(elem);
    return &mHumans[newId];
  } else {
    auto newId = mHumans.size();
    auto idInElem = elem.GetId();
    HOLGEN_FAIL_IF(idInElem != TestCompositeIdHuman::IdType(-1) && idInElem != TestCompositeIdHuman::IdType(newId), "Objects not loaded in the right order!");
    elem.SetId(newId);
    mHumansNameIndex.emplace(elem.GetName(), newId);
    return &mHumans.emplace_back(std::move(elem));
  }
}

TestCompositeIdHuman *TestCompositeIdContainer::AddHuman(TestCompositeIdHuman &elem) {
  if (mHumansNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  if (mHumansNextDeletedIndex > 0) {
    auto newId = mHumansNextDeletedIndex - 1;
    mHumansNextDeletedIndex = -1 - mHumans[newId].GetId();
    auto idInElem = elem.GetId();
    HOLGEN_FAIL_IF(idInElem != TestCompositeIdHuman::IdType(-1) && idInElem != TestCompositeIdHuman::IdType(newId), "Objects not loaded in the right order!");
    elem.SetId(newId);
    mHumansNameIndex.emplace(elem.GetName(), newId);
    elem.SetVersion(mHumans[newId].GetVersion());
    mHumans[newId] = elem;
    return &mHumans[newId];
  } else {
    auto newId = mHumans.size();
    auto idInElem = elem.GetId();
    HOLGEN_FAIL_IF(idInElem != TestCompositeIdHuman::IdType(-1) && idInElem != TestCompositeIdHuman::IdType(newId), "Objects not loaded in the right order!");
    elem.SetId(newId);
    mHumansNameIndex.emplace(elem.GetName(), newId);
    return &mHumans.emplace_back(elem);
  }
}

const TestCompositeIdHuman *TestCompositeIdContainer::GetHumanByRawIdx(int32_t idx) const {
  if (size_t(idx) >= mHumans.size())
    return nullptr;
  return &mHumans[idx];
}

TestCompositeIdHuman *TestCompositeIdContainer::GetHumanByRawIdx(int32_t idx) {
  if (size_t(idx) >= mHumans.size())
    return nullptr;
  return &mHumans[idx];
}

const TestCompositeIdHuman *TestCompositeIdContainer::GetHuman(const TestCompositeIdCompositeId &id) const {
  if (size_t(id.GetId()) >= mHumans.size())
    return nullptr;
  auto& elem = mHumans[id.GetId()];
  if (elem.GetVersion() != id.GetVersion() || !elem.IsValid()) {
    return nullptr;
  }
  return &elem;
}

TestCompositeIdHuman *TestCompositeIdContainer::GetHuman(const TestCompositeIdCompositeId &id) {
  if (size_t(id.GetId()) >= mHumans.size())
    return nullptr;
  auto& elem = mHumans[id.GetId()];
  if (elem.GetVersion() != id.GetVersion() || !elem.IsValid()) {
    return nullptr;
  }
  return &elem;
}

void TestCompositeIdContainer::DeleteHumanByRawIdx(int32_t idx) {
  auto ptr = GetHumanByRawIdx(idx);
  mHumansNameIndex.erase(ptr->GetName());
  ptr->SetVersion(ptr->GetVersion() + 1);
  auto newId = -1 - mHumansNextDeletedIndex;
  mHumansNextDeletedIndex = ptr->GetId() + 1;
  ptr->SetId(newId);
}

void TestCompositeIdContainer::DeleteHuman(const TestCompositeIdCompositeId &id) {
  HOLGEN_WARN_AND_RETURN_IF(size_t(id.GetId()) >= mHumans.size(), void(), "Out of bounds index when deleting TestCompositeIdContainer.TestCompositeIdHuman");
  HOLGEN_WARN_AND_RETURN_IF(mHumans[id.GetId()].GetVersion() != id.GetVersion(), void(), "version mismatch when deleting TestCompositeIdContainer.TestCompositeIdHuman");
  DeleteHumanByRawIdx(id.GetId());
}

size_t TestCompositeIdContainer::GetHumanCount() const {
  return mHumans.size();
}

bool TestCompositeIdContainer::operator==(const TestCompositeIdContainer &rhs) const {
  return !(
      mHumans != rhs.mHumans ||
      mHumansNameIndex != rhs.mHumansNameIndex ||
      mHumansNextDeletedIndex != rhs.mHumansNextDeletedIndex
  );
}

bool TestCompositeIdContainer::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("humans", name)) {
        auto res = JsonHelper::Parse(mHumans, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdContainer.humans field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestCompositeIdContainer: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestCompositeIdContainer!");
      auto res = JsonHelper::Parse(mHumans, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestCompositeIdContainer.humans field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestCompositeIdContainer!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestCompositeIdContainer.");
    return false;
  }
  return true;
}

rapidjson::Value TestCompositeIdContainer::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("humans", JsonHelper::Dump(mHumans, doc, luaState), doc.GetAllocator());
  return val;
}

void TestCompositeIdContainer::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestCompositeIdContainer");
  lua_setmetatable(luaState, -2);
}

void TestCompositeIdContainer::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "humans");
  LuaHelper::Push<true>(mHumans, luaState);
  lua_settable(luaState, -3);
}

void TestCompositeIdContainer::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestCompositeIdContainer *TestCompositeIdContainer::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestCompositeIdContainer", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestCompositeIdContainer *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestCompositeIdContainer TestCompositeIdContainer::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestCompositeIdContainer{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("humans", key)) {
      LuaHelper::Read(result.mHumans, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestCompositeIdContainer.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestCompositeIdContainer::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("humans", key)) {
    auto res = LuaHelper::Read(instance->mHumans, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestCompositeIdContainer.humans from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestCompositeIdContainer.{}", key);
  }
  return 0;
}

void TestCompositeIdContainer::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestCompositeIdContainer::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestCompositeIdContainer::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestCompositeIdContainer");
}

int TestCompositeIdContainer::GetHumanFromNameCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdContainer.GetHumanFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetHumanFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestCompositeIdContainer::AddHumanCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdContainer.AddHuman method with an invalid lua proxy object!");
  auto arg0 = TestCompositeIdHuman::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddHuman(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestCompositeIdContainer::GetHumanByRawIdxCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdContainer.GetHumanByRawIdx method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetHumanByRawIdx(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestCompositeIdContainer::GetHumanCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdContainer.GetHuman method with an invalid lua proxy object!");
  TestCompositeIdCompositeId arg0Mirror;
  TestCompositeIdCompositeId *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestCompositeIdCompositeId::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestCompositeIdCompositeId::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->GetHuman(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestCompositeIdContainer::DeleteHumanByRawIdxCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdContainer.DeleteHumanByRawIdx method with an invalid lua proxy object!");
  int32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteHumanByRawIdx(arg0);
  return 0;
}

int TestCompositeIdContainer::GetHumanCountCallerFromLua(lua_State *luaState) {
  auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestCompositeIdContainer.GetHumanCount method with an invalid lua proxy object!");
  auto result = instance->GetHumanCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestCompositeIdContainer::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("humans", key)) {
    auto instance = TestCompositeIdContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestCompositeIdContainer.humans with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mHumans, luaState);
  } else if (0 == strcmp("GetHumanFromName", key)) {
    lua_pushcfunction(luaState, TestCompositeIdContainer::GetHumanFromNameCallerFromLua);
  } else if (0 == strcmp("AddHuman", key)) {
    lua_pushcfunction(luaState, TestCompositeIdContainer::AddHumanCallerFromLua);
  } else if (0 == strcmp("GetHumanByRawIdx", key)) {
    lua_pushcfunction(luaState, TestCompositeIdContainer::GetHumanByRawIdxCallerFromLua);
  } else if (0 == strcmp("GetHuman", key)) {
    lua_pushcfunction(luaState, TestCompositeIdContainer::GetHumanCallerFromLua);
  } else if (0 == strcmp("DeleteHumanByRawIdx", key)) {
    lua_pushcfunction(luaState, TestCompositeIdContainer::DeleteHumanByRawIdxCallerFromLua);
  } else if (0 == strcmp("GetHumanCount", key)) {
    lua_pushcfunction(luaState, TestCompositeIdContainer::GetHumanCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestCompositeIdContainer.{}", key);
    return 0;
  }
  return 1;
}
}
