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
  TestContainerMap() = default;
  TestContainerMap(const TestContainerMap &rhs) = default;
  TestContainerMap(TestContainerMap &&rhs) = default;
  ~TestContainerMap() = default;
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
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestContainerMap *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestContainerMap ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestContainerMap &operator=(const TestContainerMap &rhs) = default;
  TestContainerMap &operator=(TestContainerMap &&rhs) = default;
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::map<uint32_t, TestContainerInnerStructWithId> mInnerStructsWithId;
  std::map<std::string, uint32_t> mInnerStructsWithIdNameIndex;
  uint32_t mInnerStructsWithIdNextId = 0;
};
}
