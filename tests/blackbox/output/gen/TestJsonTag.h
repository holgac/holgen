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
// Defined in tests/blackbox/schemas/testjson.hsc
class TestJsonTag {
public:
  TestJsonTag() = default;
  TestJsonTag(const TestJsonTag &rhs) = default;
  TestJsonTag(TestJsonTag &&rhs) noexcept = default;
  ~TestJsonTag() = default;
  bool operator==(const TestJsonTag &rhs) const;
  uint64_t GetId() const;
  const std::string &GetName() const;
  std::string &GetName();
  void SetId(uint64_t val);
  void SetName(const std::string &val);
  static TestJsonTag *Get(uint64_t id);
  static TestJsonTag *GetFromName(const std::string &key);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestJsonTag *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestJsonTag ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestJsonTag &operator=(const TestJsonTag &rhs) = default;
  TestJsonTag &operator=(TestJsonTag &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestJsonTag";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  uint64_t mId = -1;
  std::string mName;
};
}
