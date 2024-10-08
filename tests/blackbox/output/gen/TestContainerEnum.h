// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestContainerSkill.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestContainerEnum {
public:
  TestContainerEnum() = default;
  TestContainerEnum(const TestContainerEnum &rhs) = default;
  TestContainerEnum(TestContainerEnum &&rhs) = default;
  ~TestContainerEnum() = default;
  bool operator==(const TestContainerEnum &rhs) const;
  const std::array<uint8_t, 4> &GetSkills() const;
  std::array<uint8_t, 4> &GetSkills();
  void SetSkills(const std::array<uint8_t, 4> &val);
  const uint8_t &GetSkill(TestContainerSkill idx) const;
  uint8_t &GetSkill(TestContainerSkill idx);
  void SetSkill(const TestContainerSkill &idx, uint8_t val);
  size_t GetSkillCount() const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestContainerEnum *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestContainerEnum ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestContainerEnum &operator=(const TestContainerEnum &rhs) = default;
  TestContainerEnum &operator=(TestContainerEnum &&rhs) = default;
  inline static const char *CLASS_NAME = "TestContainerEnum";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int GetSkillCallerFromLua(lua_State *luaState);
  static int GetSkillCountCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::array<uint8_t, 4> mSkills;
};
}
