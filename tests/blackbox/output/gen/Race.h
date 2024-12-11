// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/person.hsc
class Race {
public:
  Race() = default;
  Race(const Race &rhs) = default;
  Race(Race &&rhs) noexcept = default;
  ~Race() = default;
  uint32_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  const std::vector<std::string> &GetHairColors() const;
  std::vector<std::string> &GetHairColors();
  const std::map<std::string, std::vector<std::string>> &GetNames() const;
  std::map<std::string, std::vector<std::string>> &GetNames();
  void SetId(uint32_t val);
  void SetName(const std::string &val);
  void SetHairColors(const std::vector<std::string> &val);
  void SetNames(const std::map<std::string, std::vector<std::string>> &val);
  bool operator==(const Race &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static Race *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static Race ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  Race &operator=(const Race &rhs) = default;
  Race &operator=(Race &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "Race";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint32_t mId;
  std::string mName;
  std::vector<std::string> mHairColors;
  std::map<std::string, std::vector<std::string>> mNames;
};
}
