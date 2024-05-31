// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <rapidjson/fwd.h>
#include "TestContainerInnerStructWithId.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestContainerMap {
public:
  bool operator==(const TestContainerMap &rhs) const;
  const std::map<uint32_t, TestContainerInnerStructWithId> &GetInnerStructsWithId() const;
  std::map<uint32_t, TestContainerInnerStructWithId> &GetInnerStructsWithId();
  void SetInnerStructsWithId(const std::map<uint32_t, TestContainerInnerStructWithId> &val);
  const TestContainerInnerStructWithId *GetInnerStructWithIdFromName(const std::string &key) const;
  TestContainerInnerStructWithId *GetInnerStructWithIdFromName(const std::string &key);
  TestContainerInnerStructWithId *AddInnerStructWithId(TestContainerInnerStructWithId &&elem);
  TestContainerInnerStructWithId *AddInnerStructWithId(TestContainerInnerStructWithId &elem);
  const TestContainerInnerStructWithId *GetInnerStructWithId(uint32_t idx) const;
  TestContainerInnerStructWithId *GetInnerStructWithId(uint32_t idx);
  bool HasInnerStructWithId(uint32_t key) const;
  void DeleteInnerStructWithId(uint32_t key);
  size_t GetInnerStructWithIdCount() const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static TestContainerMap *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::map<uint32_t, TestContainerInnerStructWithId> mInnerStructsWithId;
  std::map<std::string, uint32_t> mInnerStructsWithIdNameIndex;
  uint32_t mInnerStructsWithIdNextId = 0;
};
}
