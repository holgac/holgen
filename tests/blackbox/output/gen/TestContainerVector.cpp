// This file is generated by holgen. Do not modify manually.
#include "TestContainerVector.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const std::vector<TestContainerInnerStructWithId> &TestContainerVector::GetInnerStructsWithId() const {
  return mInnerStructsWithId;
}

std::vector<TestContainerInnerStructWithId> &TestContainerVector::GetInnerStructsWithId() {
  return mInnerStructsWithId;
}

const std::vector<TestContainerInnerStructNoId> &TestContainerVector::GetInnerStructsNoId() const {
  return mInnerStructsNoId;
}

std::vector<TestContainerInnerStructNoId> &TestContainerVector::GetInnerStructsNoId() {
  return mInnerStructsNoId;
}

const std::vector<std::string> &TestContainerVector::GetStringContainer() const {
  return mStringContainer;
}

std::vector<std::string> &TestContainerVector::GetStringContainer() {
  return mStringContainer;
}

const std::vector<uint32_t> &TestContainerVector::GetUnsignedContainer() const {
  return mUnsignedContainer;
}

std::vector<uint32_t> &TestContainerVector::GetUnsignedContainer() {
  return mUnsignedContainer;
}

void TestContainerVector::SetInnerStructsWithId(const std::vector<TestContainerInnerStructWithId> &val) {
  mInnerStructsWithId = val;
}

void TestContainerVector::SetInnerStructsNoId(const std::vector<TestContainerInnerStructNoId> &val) {
  mInnerStructsNoId = val;
}

void TestContainerVector::SetStringContainer(const std::vector<std::string> &val) {
  mStringContainer = val;
}

void TestContainerVector::SetUnsignedContainer(const std::vector<uint32_t> &val) {
  mUnsignedContainer = val;
}

const TestContainerInnerStructWithId *TestContainerVector::GetInnerStructWithIdFromName(const std::string &key) const {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId[it->second];
}

TestContainerInnerStructWithId *TestContainerVector::GetInnerStructWithIdFromName(const std::string &key) {
  auto it = mInnerStructsWithIdNameIndex.find(key);
  if (it == mInnerStructsWithIdNameIndex.end())
    return nullptr;
  return &mInnerStructsWithId[it->second];
}

const TestContainerInnerStructNoId *TestContainerVector::GetInnerStructNoIdFromName(const std::string &key) const {
  auto it = mInnerStructsNoIdNameIndex.find(key);
  if (it == mInnerStructsNoIdNameIndex.end())
    return nullptr;
  return &mInnerStructsNoId[it->second];
}

TestContainerInnerStructNoId *TestContainerVector::GetInnerStructNoIdFromName(const std::string &key) {
  auto it = mInnerStructsNoIdNameIndex.find(key);
  if (it == mInnerStructsNoIdNameIndex.end())
    return nullptr;
  return &mInnerStructsNoId[it->second];
}

TestContainerInnerStructWithId *TestContainerVector::AddInnerStructWithId(TestContainerInnerStructWithId &&elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mInnerStructsWithId.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != TestContainerInnerStructWithId::IdType(-1) && idInElem != TestContainerInnerStructWithId::IdType(newId), "Objects not loaded in the right order!");
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mInnerStructsWithId.emplace_back(std::move(elem));
  newElem.SetId(newId);
  return &newElem;
}

TestContainerInnerStructWithId *TestContainerVector::AddInnerStructWithId(TestContainerInnerStructWithId &elem) {
  if (mInnerStructsWithIdNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mInnerStructsWithId.size();
  auto idInElem = elem.GetId();
  HOLGEN_FAIL_IF(idInElem != TestContainerInnerStructWithId::IdType(-1) && idInElem != TestContainerInnerStructWithId::IdType(newId), "Objects not loaded in the right order!");
  mInnerStructsWithIdNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mInnerStructsWithId.emplace_back(elem);
  newElem.SetId(newId);
  return &newElem;
}

TestContainerInnerStructNoId *TestContainerVector::AddInnerStructNoId(TestContainerInnerStructNoId &&elem) {
  if (mInnerStructsNoIdNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mInnerStructsNoId.size();
  mInnerStructsNoIdNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mInnerStructsNoId.emplace_back(std::move(elem));
  return &newElem;
}

TestContainerInnerStructNoId *TestContainerVector::AddInnerStructNoId(const TestContainerInnerStructNoId &elem) {
  if (mInnerStructsNoIdNameIndex.contains(elem.GetName())) {
    return nullptr;
  }
  auto newId = mInnerStructsNoId.size();
  mInnerStructsNoIdNameIndex.emplace(elem.GetName(), newId);
  auto &newElem = mInnerStructsNoId.emplace_back(elem);
  return &newElem;
}

std::string *TestContainerVector::AddStringElem(std::string &&elem) {
  return &mStringContainer.emplace_back(std::move(elem));
}

std::string *TestContainerVector::AddStringElem(const std::string &elem) {
  return &mStringContainer.emplace_back(elem);
}

uint32_t *TestContainerVector::AddUnsignedElem(uint32_t elem) {
  return &mUnsignedContainer.emplace_back(elem);
}

const TestContainerInnerStructWithId *TestContainerVector::GetInnerStructWithId(uint32_t idx) const {
  if (size_t(idx) >= mInnerStructsWithId.size())
    return nullptr;
  return &mInnerStructsWithId[idx];
}

TestContainerInnerStructWithId *TestContainerVector::GetInnerStructWithId(uint32_t idx) {
  if (size_t(idx) >= mInnerStructsWithId.size())
    return nullptr;
  return &mInnerStructsWithId[idx];
}

const TestContainerInnerStructNoId *TestContainerVector::GetInnerStructNoId(size_t idx) const {
  if (size_t(idx) >= mInnerStructsNoId.size())
    return nullptr;
  return &mInnerStructsNoId[idx];
}

TestContainerInnerStructNoId *TestContainerVector::GetInnerStructNoId(size_t idx) {
  if (size_t(idx) >= mInnerStructsNoId.size())
    return nullptr;
  return &mInnerStructsNoId[idx];
}

const std::string *TestContainerVector::GetStringElem(size_t idx) const {
  if (size_t(idx) >= mStringContainer.size())
    return nullptr;
  return &mStringContainer[idx];
}

std::string *TestContainerVector::GetStringElem(size_t idx) {
  if (size_t(idx) >= mStringContainer.size())
    return nullptr;
  return &mStringContainer[idx];
}

const uint32_t *TestContainerVector::GetUnsignedElem(size_t idx) const {
  if (size_t(idx) >= mUnsignedContainer.size())
    return nullptr;
  return &mUnsignedContainer[idx];
}

uint32_t *TestContainerVector::GetUnsignedElem(size_t idx) {
  if (size_t(idx) >= mUnsignedContainer.size())
    return nullptr;
  return &mUnsignedContainer[idx];
}

void TestContainerVector::DeleteInnerStructWithId(uint32_t idx) {
  auto ptr = GetInnerStructWithId(idx);
  mInnerStructsWithIdNameIndex.erase(ptr->GetName());
  if (size_t(idx) != mInnerStructsWithId.size() - 1) {
    mInnerStructsWithIdNameIndex.at(mInnerStructsWithId.back().GetName()) = idx;
    mInnerStructsWithId[idx] = std::move(mInnerStructsWithId.back());
  }
  mInnerStructsWithId.pop_back();
}

void TestContainerVector::DeleteInnerStructNoId(size_t idx) {
  auto ptr = GetInnerStructNoId(idx);
  mInnerStructsNoIdNameIndex.erase(ptr->GetName());
  if (size_t(idx) != mInnerStructsNoId.size() - 1) {
    mInnerStructsNoIdNameIndex.at(mInnerStructsNoId.back().GetName()) = idx;
    mInnerStructsNoId[idx] = std::move(mInnerStructsNoId.back());
  }
  mInnerStructsNoId.pop_back();
}

void TestContainerVector::DeleteStringElem(size_t idx) {
  if (size_t(idx) != mStringContainer.size() - 1) {
    mStringContainer[idx] = std::move(mStringContainer.back());
  }
  mStringContainer.pop_back();
}

void TestContainerVector::DeleteUnsignedElem(size_t idx) {
  if (size_t(idx) != mUnsignedContainer.size() - 1) {
    mUnsignedContainer[idx] = std::move(mUnsignedContainer.back());
  }
  mUnsignedContainer.pop_back();
}

size_t TestContainerVector::GetInnerStructWithIdCount() const {
  return mInnerStructsWithId.size();
}

size_t TestContainerVector::GetInnerStructNoIdCount() const {
  return mInnerStructsNoId.size();
}

size_t TestContainerVector::GetStringElemCount() const {
  return mStringContainer.size();
}

size_t TestContainerVector::GetUnsignedElemCount() const {
  return mUnsignedContainer.size();
}

bool TestContainerVector::operator==(const TestContainerVector &rhs) const {
  return !(
      mInnerStructsWithId != rhs.mInnerStructsWithId ||
      mInnerStructsNoId != rhs.mInnerStructsNoId ||
      mStringContainer != rhs.mStringContainer ||
      mUnsignedContainer != rhs.mUnsignedContainer ||
      mInnerStructsWithIdNameIndex != rhs.mInnerStructsWithIdNameIndex ||
      mInnerStructsNoIdNameIndex != rhs.mInnerStructsNoIdNameIndex
  );
}

bool TestContainerVector::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("innerStructsWithId", name)) {
        auto res = JsonHelper::Parse(mInnerStructsWithId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.innerStructsWithId field");
      } else if (0 == strcmp("innerStructsNoId", name)) {
        auto res = JsonHelper::Parse(mInnerStructsNoId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.innerStructsNoId field");
      } else if (0 == strcmp("stringContainer", name)) {
        auto res = JsonHelper::Parse(mStringContainer, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.stringContainer field");
      } else if (0 == strcmp("unsignedContainer", name)) {
        auto res = JsonHelper::Parse(mUnsignedContainer, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.unsignedContainer field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing TestContainerVector: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerVector!");
      auto res = JsonHelper::Parse(mInnerStructsWithId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.innerStructsWithId field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerVector!");
      auto res = JsonHelper::Parse(mInnerStructsNoId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.innerStructsNoId field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerVector!");
      auto res = JsonHelper::Parse(mStringContainer, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.stringContainer field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing TestContainerVector!");
      auto res = JsonHelper::Parse(mUnsignedContainer, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse TestContainerVector.unsignedContainer field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing TestContainerVector!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing TestContainerVector.");
    return false;
  }
  return true;
}

rapidjson::Value TestContainerVector::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("innerStructsWithId", JsonHelper::Dump(mInnerStructsWithId, doc, luaState), doc.GetAllocator());
  val.AddMember("innerStructsNoId", JsonHelper::Dump(mInnerStructsNoId, doc, luaState), doc.GetAllocator());
  val.AddMember("stringContainer", JsonHelper::Dump(mStringContainer, doc, luaState), doc.GetAllocator());
  val.AddMember("unsignedContainer", JsonHelper::Dump(mUnsignedContainer, doc, luaState), doc.GetAllocator());
  return val;
}

void TestContainerVector::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "TestContainerVector");
  lua_setmetatable(luaState, -2);
}

void TestContainerVector::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "innerStructsWithId");
  LuaHelper::Push<true>(mInnerStructsWithId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "innerStructsNoId");
  LuaHelper::Push<true>(mInnerStructsNoId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "stringContainer");
  LuaHelper::Push<true>(mStringContainer, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "unsignedContainer");
  LuaHelper::Push<true>(mUnsignedContainer, luaState);
  lua_settable(luaState, -3);
}

void TestContainerVector::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

TestContainerVector *TestContainerVector::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting TestContainerVector", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (TestContainerVector *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

TestContainerVector TestContainerVector::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = TestContainerVector{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("innerStructsWithId", key)) {
      LuaHelper::Read(result.mInnerStructsWithId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("innerStructsNoId", key)) {
      LuaHelper::Read(result.mInnerStructsNoId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("stringContainer", key)) {
      LuaHelper::Read(result.mStringContainer, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("unsignedContainer", key)) {
      LuaHelper::Read(result.mUnsignedContainer, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: TestContainerVector.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int TestContainerVector::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("innerStructsWithId", key)) {
    auto res = LuaHelper::Read(instance->mInnerStructsWithId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerVector.innerStructsWithId from lua failed!");
  } else if (0 == strcmp("innerStructsNoId", key)) {
    auto res = LuaHelper::Read(instance->mInnerStructsNoId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerVector.innerStructsNoId from lua failed!");
  } else if (0 == strcmp("stringContainer", key)) {
    auto res = LuaHelper::Read(instance->mStringContainer, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerVector.stringContainer from lua failed!");
  } else if (0 == strcmp("unsignedContainer", key)) {
    auto res = LuaHelper::Read(instance->mUnsignedContainer, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning TestContainerVector.unsignedContainer from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerVector.{}", key);
  }
  return 0;
}

int TestContainerVector::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.operator== method with an invalid lua proxy object!");
  TestContainerVector arg0Mirror;
  TestContainerVector *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestContainerVector::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestContainerVector::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void TestContainerVector::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, TestContainerVector::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, TestContainerVector::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, TestContainerVector::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "TestContainerVector");
}

int TestContainerVector::GetInnerStructWithIdFromNameCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetInnerStructWithIdFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetInnerStructWithIdFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::GetInnerStructNoIdFromNameCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetInnerStructNoIdFromName method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetInnerStructNoIdFromName(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::AddInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.AddInnerStructWithId method with an invalid lua proxy object!");
  auto arg0 = TestContainerInnerStructWithId::ReadProxyFromLua(luaState, -1);
  auto result = instance->AddInnerStructWithId(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::AddInnerStructNoIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.AddInnerStructNoId method with an invalid lua proxy object!");
  TestContainerInnerStructNoId arg0Mirror;
  TestContainerInnerStructNoId *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = TestContainerInnerStructNoId::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = TestContainerInnerStructNoId::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->AddInnerStructNoId(*arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::AddStringElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.AddStringElem method with an invalid lua proxy object!");
  std::string arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->AddStringElem(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::AddUnsignedElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.AddUnsignedElem method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->AddUnsignedElem(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::GetInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetInnerStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetInnerStructWithId(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::GetInnerStructNoIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetInnerStructNoId method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetInnerStructNoId(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::GetStringElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetStringElem method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetStringElem(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::GetUnsignedElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetUnsignedElem method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  auto result = instance->GetUnsignedElem(arg0);
  LuaHelper::Push<false>(result, luaState);
  return 1;
}

int TestContainerVector::DeleteInnerStructWithIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.DeleteInnerStructWithId method with an invalid lua proxy object!");
  uint32_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteInnerStructWithId(arg0);
  return 0;
}

int TestContainerVector::DeleteInnerStructNoIdCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.DeleteInnerStructNoId method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteInnerStructNoId(arg0);
  return 0;
}

int TestContainerVector::DeleteStringElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.DeleteStringElem method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteStringElem(arg0);
  return 0;
}

int TestContainerVector::DeleteUnsignedElemCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.DeleteUnsignedElem method with an invalid lua proxy object!");
  size_t arg0;
  LuaHelper::Read(arg0, luaState, -1);
  instance->DeleteUnsignedElem(arg0);
  return 0;
}

int TestContainerVector::GetInnerStructWithIdCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetInnerStructWithIdCount method with an invalid lua proxy object!");
  auto result = instance->GetInnerStructWithIdCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestContainerVector::GetInnerStructNoIdCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetInnerStructNoIdCount method with an invalid lua proxy object!");
  auto result = instance->GetInnerStructNoIdCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestContainerVector::GetStringElemCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetStringElemCount method with an invalid lua proxy object!");
  auto result = instance->GetStringElemCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestContainerVector::GetUnsignedElemCountCallerFromLua(lua_State *luaState) {
  auto instance = TestContainerVector::ReadProxyFromLua(luaState, -1);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling TestContainerVector.GetUnsignedElemCount method with an invalid lua proxy object!");
  auto result = instance->GetUnsignedElemCount();
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

int TestContainerVector::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("innerStructsWithId", key)) {
    auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerVector.innerStructsWithId with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mInnerStructsWithId, luaState);
  } else if (0 == strcmp("innerStructsNoId", key)) {
    auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerVector.innerStructsNoId with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mInnerStructsNoId, luaState);
  } else if (0 == strcmp("stringContainer", key)) {
    auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerVector.stringContainer with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mStringContainer, luaState);
  } else if (0 == strcmp("unsignedContainer", key)) {
    auto instance = TestContainerVector::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for TestContainerVector.unsignedContainer with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mUnsignedContainer, luaState);
  } else if (0 == strcmp("GetInnerStructWithIdFromName", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetInnerStructWithIdFromNameCallerFromLua);
  } else if (0 == strcmp("GetInnerStructNoIdFromName", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetInnerStructNoIdFromNameCallerFromLua);
  } else if (0 == strcmp("AddInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerVector::AddInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("AddInnerStructNoId", key)) {
    lua_pushcfunction(luaState, TestContainerVector::AddInnerStructNoIdCallerFromLua);
  } else if (0 == strcmp("AddStringElem", key)) {
    lua_pushcfunction(luaState, TestContainerVector::AddStringElemCallerFromLua);
  } else if (0 == strcmp("AddUnsignedElem", key)) {
    lua_pushcfunction(luaState, TestContainerVector::AddUnsignedElemCallerFromLua);
  } else if (0 == strcmp("GetInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("GetInnerStructNoId", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetInnerStructNoIdCallerFromLua);
  } else if (0 == strcmp("GetStringElem", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetStringElemCallerFromLua);
  } else if (0 == strcmp("GetUnsignedElem", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetUnsignedElemCallerFromLua);
  } else if (0 == strcmp("DeleteInnerStructWithId", key)) {
    lua_pushcfunction(luaState, TestContainerVector::DeleteInnerStructWithIdCallerFromLua);
  } else if (0 == strcmp("DeleteInnerStructNoId", key)) {
    lua_pushcfunction(luaState, TestContainerVector::DeleteInnerStructNoIdCallerFromLua);
  } else if (0 == strcmp("DeleteStringElem", key)) {
    lua_pushcfunction(luaState, TestContainerVector::DeleteStringElemCallerFromLua);
  } else if (0 == strcmp("DeleteUnsignedElem", key)) {
    lua_pushcfunction(luaState, TestContainerVector::DeleteUnsignedElemCallerFromLua);
  } else if (0 == strcmp("GetInnerStructWithIdCount", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetInnerStructWithIdCountCallerFromLua);
  } else if (0 == strcmp("GetInnerStructNoIdCount", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetInnerStructNoIdCountCallerFromLua);
  } else if (0 == strcmp("GetStringElemCount", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetStringElemCountCallerFromLua);
  } else if (0 == strcmp("GetUnsignedElemCount", key)) {
    lua_pushcfunction(luaState, TestContainerVector::GetUnsignedElemCountCallerFromLua);
  } else {
    HOLGEN_WARN("Unexpected lua field: TestContainerVector.{}", key);
    return 0;
  }
  return 1;
}
}
