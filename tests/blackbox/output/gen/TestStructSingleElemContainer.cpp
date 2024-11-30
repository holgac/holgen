// This file is generated by holgen. Do not modify manually.
#include "TestStructSingleElemContainer.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<TestStructSingleElem> &TestStructSingleElemContainer::GetSingleElemStructs() const {
  return mSingleElemStructs;
}

std::vector<TestStructSingleElem> &TestStructSingleElemContainer::GetSingleElemStructs() {
  return mSingleElemStructs;
}

const std::vector<TestStructSingleElemWithId> &TestStructSingleElemContainer::GetSingleElemStructsWithId() const {
  return mSingleElemStructsWithId;
}

std::vector<TestStructSingleElemWithId> &TestStructSingleElemContainer::GetSingleElemStructsWithId() {
  return mSingleElemStructsWithId;
}

void TestStructSingleElemContainer::SetSingleElemStructs(const std::vector<TestStructSingleElem> &val) {
  mSingleElemStructs = val;
}

void TestStructSingleElemContainer::SetSingleElemStructsWithId(const std::vector<TestStructSingleElemWithId> &val) {
  mSingleElemStructsWithId = val;
}

TestStructSingleElem *TestStructSingleElemContainer::AddSingleElemStruct(TestStructSingleElem &&elem) {
  return &mSingleElemStructs.emplace_back(std::move(elem));
}

TestStructSingleElem *TestStructSingleElemContainer::AddSingleElemStruct(const TestStructSingleElem &elem) {
  return &mSingleElemStructs.emplace_back(elem);
}

TestStructSingleElemWithId *TestStructSingleElemContainer::AddSingleElemStructWithId(TestStructSingleElemWithId &&elem) {
  auto newId = mSingleElemStructsWithId.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != TestStructSingleElemWithId::IdType(-1) && idInElem != TestStructSingleElemWithId::IdType(newId), "Objects not loaded in the right order!");
  elem.SetId(newId);
  return &mSingleElemStructsWithId.emplace_back(std::move(elem));
}

TestStructSingleElemWithId *TestStructSingleElemContainer::AddSingleElemStructWithId(TestStructSingleElemWithId &elem) {
  auto newId = mSingleElemStructsWithId.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != TestStructSingleElemWithId::IdType(-1) && idInElem != TestStructSingleElemWithId::IdType(newId), "Objects not loaded in the right order!");
  elem.SetId(newId);
  return &mSingleElemStructsWithId.emplace_back(elem);
}

const TestStructSingleElem *TestStructSingleElemContainer::GetSingleElemStruct(size_t idx) const {
  if (size_t(idx) >= mSingleElemStructs.size())
    return nullptr;
  return &mSingleElemStructs[idx];
}

TestStructSingleElem *TestStructSingleElemContainer::GetSingleElemStruct(size_t idx) {
  if (size_t(idx) >= mSingleElemStructs.size())
    return nullptr;
  return &mSingleElemStructs[idx];
}

const TestStructSingleElemWithId *TestStructSingleElemContainer::GetSingleElemStructWithId(uint32_t idx) const {
  if (size_t(idx) >= mSingleElemStructsWithId.size())
    return nullptr;
  return &mSingleElemStructsWithId[idx];
}

TestStructSingleElemWithId *TestStructSingleElemContainer::GetSingleElemStructWithId(uint32_t idx) {
  if (size_t(idx) >= mSingleElemStructsWithId.size())
    return nullptr;
  return &mSingleElemStructsWithId[idx];
}

void TestStructSingleElemContainer::DeleteSingleElemStruct(size_t idx) {
  if (idx != mSingleElemStructs.size() - 1) {
    mSingleElemStructs[idx] = std::move(mSingleElemStructs.back());
  }
  mSingleElemStructs.pop_back();
}

size_t TestStructSingleElemContainer::GetSingleElemStructCount() const {
  return mSingleElemStructs.size();
}

size_t TestStructSingleElemContainer::GetSingleElemStructWithIdCount() const {
  return mSingleElemStructsWithId.size();
}

bool TestStructSingleElemContainer::operator==(const TestStructSingleElemContainer &rhs) const {
  return !(
      mSingleElemStructs != rhs.mSingleElemStructs ||
      mSingleElemStructsWithId != rhs.mSingleElemStructsWithId
  );
}

bool TestStructSingleElemContainer::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("singleElemStructs", name)) {
        auto res = JsonHelper::Parse(mSingleElemStructs, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemContainer.singleElemStructs field");
      } else if (0 == strcmp("singleElemStructsWithId", name)) {
        auto res = JsonHelper::Parse(mSingleElemStructsWithId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemContainer.singleElemStructsWithId field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestStructSingleElemContainer: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructSingleElemContainer!");
      auto res = JsonHelper::Parse(mSingleElemStructs, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemContainer.singleElemStructs field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestStructSingleElemContainer!");
      auto res = JsonHelper::Parse(mSingleElemStructsWithId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestStructSingleElemContainer.singleElemStructsWithId field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestStructSingleElemContainer!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestStructSingleElemContainer.");
    return false;
  }
  return true;
}

rapidjson::Value TestStructSingleElemContainer::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("singleElemStructs", JsonHelper::Dump(mSingleElemStructs, doc, luaState), doc.GetAllocator());
  val.AddMember("singleElemStructsWithId", JsonHelper::Dump(mSingleElemStructsWithId, doc, luaState), doc.GetAllocator());
  return val;
}

void TestStructSingleElemContainer::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestStructSingleElemContainer");
  lua_setmetatable(luaState, -2);
}

void TestStructSingleElemContainer::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "singleElemStructs");
  LuaHelper::Push<true>(mSingleElemStructs, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "singleElemStructsWithId");
  LuaHelper::Push<true>(mSingleElemStructsWithId, luaState);
  lua_settable(luaState, -3);
}

void TestStructSingleElemContainer::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestStructSingleElemContainer *TestStructSingleElemContainer::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestStructSingleElemContainer", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestStructSingleElemContainer *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestStructSingleElemContainer TestStructSingleElemContainer::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestStructSingleElemContainer{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("singleElemStructs", key)) {
      LuaHelper::Read(result.mSingleElemStructs, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("singleElemStructsWithId", key)) {
      LuaHelper::Read(result.mSingleElemStructsWithId, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestStructSingleElemContainer::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("singleElemStructs", key)) {
    auto res = LuaHelper::Read(instance->mSingleElemStructs, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructSingleElemContainer.singleElemStructs from lua failed!");
  } else if (0 == strcmp("singleElemStructsWithId", key)) {
    auto res = LuaHelper::Read(instance->mSingleElemStructsWithId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestStructSingleElemContainer.singleElemStructsWithId from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
  }
  return 0;
}

void TestStructSingleElemContainer::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestStructSingleElemContainer::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestStructSingleElemContainer::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestStructSingleElemContainer");
}

int TestStructSingleElemContainer::AddSingleElemStructCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.AddSingleElemStruct method with an invalid lua proxy object!");
  TestStructSingleElem arg0Mirror;
  TestStructSingleElem *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestStructSingleElem::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestStructSingleElem::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->AddSingleElemStruct(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestStructSingleElemContainer::AddSingleElemStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.AddSingleElemStructWithId method with an invalid lua proxy object!");
  auto arg0 = TestStructSingleElemWithId::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddSingleElemStructWithId(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestStructSingleElemContainer::GetSingleElemStructCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStruct method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetSingleElemStruct(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestStructSingleElemContainer::GetSingleElemStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetSingleElemStructWithId(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestStructSingleElemContainer::DeleteSingleElemStructCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.DeleteSingleElemStruct method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteSingleElemStruct(arg0);
  return 0;
}

int TestStructSingleElemContainer::GetSingleElemStructCountCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStructCount method with an invalid lua proxy object!");
  auto result = instance->GetSingleElemStructCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestStructSingleElemContainer::GetSingleElemStructWithIdCountCallerFromLua(lua_State *luaState) {
  auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestStructSingleElemContainer.GetSingleElemStructWithIdCount method with an invalid lua proxy object!");
  auto result = instance->GetSingleElemStructWithIdCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestStructSingleElemContainer::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("singleElemStructs", key)) {
    auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSingleElemContainer.singleElemStructs with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mSingleElemStructs, luaState);
  } else if (0 == strcmp("singleElemStructsWithId", key)) {
    auto instance = TestStructSingleElemContainer::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestStructSingleElemContainer.singleElemStructsWithId with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mSingleElemStructsWithId, luaState);
  } else if (0 == strcmp("AddSingleElemStruct", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::AddSingleElemStructCallerFromLua);
  } else if (0 == strcmp("AddSingleElemStructWithId", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::AddSingleElemStructWithIdCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStruct", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStructWithId", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructWithIdCallerFromLua);
  } else if (0 == strcmp("DeleteSingleElemStruct", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::DeleteSingleElemStructCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStructCount", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructCountCallerFromLua);
  } else if (0 == strcmp("GetSingleElemStructWithIdCount", key)) {
    lua_pushcfunction(luaState, TestStructSingleElemContainer::GetSingleElemStructWithIdCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestStructSingleElemContainer.{}", key);
    return 0;
  }
  return 1;
}
}
