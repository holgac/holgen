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
class Human {
public:
  bool operator==(const Human &rhs) const;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  static Human *Get(uint32_t id);
  static Human *GetFromName(const std::string &key);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static Human *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  uint32_t mId = -1;
  std::string mName;
};
}
