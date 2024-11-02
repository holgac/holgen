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
// Defined in tests/blackbox/schemas/ref.hsc
class Human {
public:
  Human() = default;
  Human(const Human &rhs) = default;
  Human(Human &&rhs) noexcept = default;
  ~Human() = default;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  static Human *Get(uint32_t id);
  static Human *GetFromName(const std::string &key);
  bool operator==(const Human &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Human *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Human ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Human &operator=(const Human &rhs) = default;
  Human &operator=(Human &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Human";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mId = -1;
  std::string mName;
};
}
