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
class TestCompositeIdDog {
public:
  using IdType=int32_t;
  TestCompositeIdDog() = default;
  TestCompositeIdDog(const TestCompositeIdDog &rhs) = default;
  TestCompositeIdDog(TestCompositeIdDog &&rhs) noexcept = default;
  ~TestCompositeIdDog() = default;
  int32_t GetId() const;
  int16_t GetVersion() const;
  const std::string &GetBreed() const;
  std::string &GetBreed();
  void SetId(int32_t val);
  void SetVersion(int16_t val);
  void SetBreed(const std::string &val);
  TestCompositeIdCompositeId GetCompositeId() const;
  bool IsValid() const;
  bool operator==(const TestCompositeIdDog &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestCompositeIdDog *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestCompositeIdDog ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestCompositeIdDog &operator=(const TestCompositeIdDog &rhs) = default;
  TestCompositeIdDog &operator=(TestCompositeIdDog &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestCompositeIdDog";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  int32_t mId = -1;
  int16_t mVersion = 0;
  std::string mBreed;
};
}
