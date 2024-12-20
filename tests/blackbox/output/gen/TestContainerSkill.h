// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <format>
#include <lua.hpp>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testcontainer.hsc
class TestContainerSkill {
public:
  using UnderlyingType=uint8_t;
  enum Entry : UnderlyingType {
    Strength = 0,
    Perception = 1,
    Endurance = 2,
    Charisma = 3,
  };
  explicit TestContainerSkill(UnderlyingType value = Invalid);
  TestContainerSkill(Entry value);
  TestContainerSkill::Entry GetValue() const;
  static TestContainerSkill FromString(std::string_view str);
  const char *ToString() const;
  TestContainerSkill &operator=(UnderlyingType rhs);
  bool operator==(UnderlyingType rhs) const;
  bool operator==(const TestContainerSkill &rhs) const;
  bool operator!=(UnderlyingType rhs) const;
  bool operator!=(const TestContainerSkill &rhs) const;
  bool operator<(UnderlyingType rhs) const;
  bool operator<(const TestContainerSkill &rhs) const;
  constexpr static std::array<TestContainerSkill::Entry, 4> GetEntries() {
    return std::array<TestContainerSkill::Entry, 4>{Strength, Perception, Endurance, Charisma};
  }
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestContainerSkill ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void PushEnumToLua(lua_State *luaState);
  static constexpr const UnderlyingType Invalid = 4;
private:
  TestContainerSkill::Entry mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_test::TestContainerSkill> {
public:
  size_t operator()(const holgen_blackbox_test::TestContainerSkill &obj) const {
    return std::hash<holgen_blackbox_test::TestContainerSkill::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestContainerSkill::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestContainerSkill::Entry &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", holgen_blackbox_test::TestContainerSkill(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestContainerSkill> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestContainerSkill &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
#define holgen_blackbox_test_FOR_EACH_TestContainerSkill \
    holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Strength) \
    holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Perception) \
    holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Endurance) \
    holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Charisma)
#define holgen_blackbox_test_SWITCH_TestContainerSkill(VALUE) \
    switch ((VALUE)) { \
    case holgen_blackbox_test::TestContainerSkill::Strength: \
      holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Strength) \
    case holgen_blackbox_test::TestContainerSkill::Perception: \
      holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Perception) \
    case holgen_blackbox_test::TestContainerSkill::Endurance: \
      holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Endurance) \
    case holgen_blackbox_test::TestContainerSkill::Charisma: \
      holgen_blackbox_test_FOR_EACH_TestContainerSkill_DOER(Charisma) \
    }
