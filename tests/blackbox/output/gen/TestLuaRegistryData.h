// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <functional>
#include <string>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestLuaRegistryData {
public:
  TestLuaRegistryData() = default;
  TestLuaRegistryData(const TestLuaRegistryData &rhs) = delete;
  TestLuaRegistryData(TestLuaRegistryData &&rhs);
  ~TestLuaRegistryData();
  void InitializeLua(lua_State *luaState);
  void UninitializeLua(lua_State *luaState);
  bool operator==(const TestLuaRegistryData &rhs) const;
  int GetData() const;
  void SetData(int val);
  void SetTable(std::string val);
  const std::string &GetTable();
  void Init(lua_State *luaState, const std::function<void(lua_State *)> &initData) const;
  int32_t Get(lua_State *luaState) const;
  void Add(lua_State *luaState, const int32_t val) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestLuaRegistryData *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestLuaRegistryData ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestLuaRegistryData &operator=(const TestLuaRegistryData &rhs) = delete;
  TestLuaRegistryData &operator=(TestLuaRegistryData &&rhs);
  inline static const char *CLASS_NAME = "TestLuaRegistryData";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int InitCallerFromLua(lua_State *luaState);
  static int GetCallerFromLua(lua_State *luaState);
  static int AddCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  int mData = LUA_NOREF;
  std::string mTable;
};
}
