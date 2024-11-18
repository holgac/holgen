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
// Defined in tests/blackbox/schemas/gamedata.hsc
class Boot {
public:
  Boot() = default;
  Boot(const Boot &rhs) = default;
  Boot(Boot &&rhs) noexcept = default;
  ~Boot() = default;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  const std::string &GetColor() const;
  std::string &GetColor();
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  void SetColor(const std::string &val);
  static Boot *Get(uint32_t id);
  static Boot *GetFromName(const std::string &key);
  bool operator==(const Boot &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Boot *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Boot ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Boot &operator=(const Boot &rhs) = default;
  Boot &operator=(Boot &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Boot";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mId = -1;
  std::string mName;
  std::string mColor;
};
}
