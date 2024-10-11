// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/lua.hsc
class LuaTestStructContainer {
public:
  LuaTestStructContainer() = default;
  LuaTestStructContainer(const LuaTestStructContainer &rhs) = default;
  LuaTestStructContainer(LuaTestStructContainer &&rhs) noexcept = default;
  ~LuaTestStructContainer() = default;
  bool operator==(const LuaTestStructContainer &rhs) const;
  const std::vector<uint32_t> &GetTestVector() const;
  std::vector<uint32_t> &GetTestVector();
  const std::map<std::string, uint32_t> &GetTestMap() const;
  std::map<std::string, uint32_t> &GetTestMap();
  void SetTestVector(const std::vector<uint32_t> &val);
  void SetTestMap(const std::map<std::string, uint32_t> &val);
  uint32_t *AddTestVectorElem(uint32_t elem);
  const uint32_t *GetTestVectorElem(size_t idx) const;
  uint32_t *GetTestVectorElem(size_t idx);
  void DeleteTestVectorElem(size_t idx);
  size_t GetTestVectorElemCount() const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static LuaTestStructContainer *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static LuaTestStructContainer ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  LuaTestStructContainer &operator=(const LuaTestStructContainer &rhs) = default;
  LuaTestStructContainer &operator=(LuaTestStructContainer &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "LuaTestStructContainer";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int AddTestVectorElemCallerFromLua(lua_State *luaState);
  static int GetTestVectorElemCallerFromLua(lua_State *luaState);
  static int DeleteTestVectorElemCallerFromLua(lua_State *luaState);
  static int GetTestVectorElemCountCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::vector<uint32_t> mTestVector;
  std::map<std::string, uint32_t> mTestMap;
};
}
