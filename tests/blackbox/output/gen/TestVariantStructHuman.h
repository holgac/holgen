// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestVariantStructHuman {
public:
  bool operator==(const TestVariantStructHuman &rhs) const;
  const std::string &GetName() const;
  std::string &GetName();
  const std::string &GetNationality() const;
  std::string &GetNationality();
  void SetName(const std::string &val);
  void SetNationality(const std::string &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestVariantStructHuman *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestVariantStructHuman ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::string mName = "Jean";
  std::string mNationality = "Mixed";
};
}
