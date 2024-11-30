// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <deque>
#include <map>
#include <string>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "TestCompositeIdHuman.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testcompositeid.hsc
class TestCompositeIdContainer {
public:
  TestCompositeIdContainer() = default;
  TestCompositeIdContainer(const TestCompositeIdContainer &rhs) = default;
  TestCompositeIdContainer(TestCompositeIdContainer &&rhs) noexcept = default;
  ~TestCompositeIdContainer() = default;
  const std::deque<TestCompositeIdHuman> &GetHumans() const;
  std::deque<TestCompositeIdHuman> &GetHumans();
  void SetHumans(const std::deque<TestCompositeIdHuman> &val);
  const TestCompositeIdHuman *GetHumanFromName(const std::string &key) const;
  TestCompositeIdHuman *GetHumanFromName(const std::string &key);
  TestCompositeIdHuman *AddHuman(TestCompositeIdHuman &&elem);
  TestCompositeIdHuman *AddHuman(TestCompositeIdHuman &elem);
  const TestCompositeIdHuman *GetHuman(int32_t idx) const;
  TestCompositeIdHuman *GetHuman(int32_t idx);
  void DeleteHuman(int32_t idx);
  size_t GetHumanCount() const;
  bool operator==(const TestCompositeIdContainer &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestCompositeIdContainer *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestCompositeIdContainer ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestCompositeIdContainer &operator=(const TestCompositeIdContainer &rhs) = default;
  TestCompositeIdContainer &operator=(TestCompositeIdContainer &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestCompositeIdContainer";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int GetHumanFromNameCallerFromLua(lua_State *luaState);
  static int AddHumanCallerFromLua(lua_State *luaState);
  static int GetHumanCallerFromLua(lua_State *luaState);
  static int DeleteHumanCallerFromLua(lua_State *luaState);
  static int GetHumanCountCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::deque<TestCompositeIdHuman> mHumans;
  std::map<std::string, int32_t> mHumansNameIndex;
  uint32_t mHumansDeletedCount = 0;
  int32_t mHumansNextDeletedIndex = -1;
};
}
