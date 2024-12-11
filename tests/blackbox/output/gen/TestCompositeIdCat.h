// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <lua.hpp>
#include <rapidjson/fwd.h>
#include "TestCompositeIdCompositeId.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testcompositeid.hsc
class TestCompositeIdCat {
public:
  using IdType=int32_t;
  TestCompositeIdCat() = default;
  TestCompositeIdCat(const TestCompositeIdCat &rhs) = default;
  TestCompositeIdCat(TestCompositeIdCat &&rhs) noexcept = default;
  ~TestCompositeIdCat() = default;
  int32_t GetId() const;
  int16_t GetVersion() const;
  const std::string &GetName() const;
  std::string &GetName();
  const std::string &GetBreed() const;
  std::string &GetBreed();
  void SetId(int32_t val);
  void SetVersion(int16_t val);
  void SetName(const std::string &val);
  void SetBreed(const std::string &val);
  TestCompositeIdCompositeId GetCompositeId() const;
  bool IsValid() const;
  bool operator==(const TestCompositeIdCat &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestCompositeIdCat *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestCompositeIdCat ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestCompositeIdCat &operator=(const TestCompositeIdCat &rhs) = default;
  TestCompositeIdCat &operator=(TestCompositeIdCat &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestCompositeIdCat";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int EqualsOperatorCallerFromLua(lua_State *luaState);
  static int GetCompositeIdCallerFromLua(lua_State *luaState);
  static int IsValidCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  int32_t mId = -1;
  int16_t mVersion = 0;
  std::string mName;
  std::string mBreed;
};
}
