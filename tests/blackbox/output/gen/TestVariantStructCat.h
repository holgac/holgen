// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestVariantStructCat {
public:
  bool operator==(const TestVariantStructCat &rhs) const;
  const std::string &GetName() const;
  std::string &GetName();
  const std::string &GetColor() const;
  std::string &GetColor();
  void SetName(const std::string &val);
  void SetColor(const std::string &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static TestVariantStructCat *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::string mName = "Whiskers";
  std::string mColor = "orange";
};
}
