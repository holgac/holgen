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
// Defined in tests/blackbox/schemas/testcompositeid.hsc
class TestCompositeIdObjectType {
public:
  using UnderlyingType=uint8_t;
  enum Entry : UnderlyingType {
    Human = 0,
    Cat = 1,
    Dog = 2,
  };
  explicit TestCompositeIdObjectType(UnderlyingType value = Invalid);
  TestCompositeIdObjectType(Entry value);
  TestCompositeIdObjectType::Entry GetValue() const;
  static TestCompositeIdObjectType FromString(std::string_view str);
  const char *ToString() const;
  TestCompositeIdObjectType &operator=(UnderlyingType rhs);
  bool operator==(UnderlyingType rhs) const;
  bool operator==(const TestCompositeIdObjectType &rhs) const;
  bool operator!=(UnderlyingType rhs) const;
  bool operator!=(const TestCompositeIdObjectType &rhs) const;
  bool operator<(UnderlyingType rhs) const;
  bool operator<(const TestCompositeIdObjectType &rhs) const;
  constexpr static std::array<TestCompositeIdObjectType::Entry, 3> GetEntries() {
    return std::array<TestCompositeIdObjectType::Entry, 3>{Human, Cat, Dog};
  }
  bool ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState);
  rapidjson::Value DumpJson(rapidjson::Document &doc, lua_State *luaState) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestCompositeIdObjectType ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void PushEnumToLua(lua_State *luaState);
  static constexpr const UnderlyingType Invalid = 3;
private:
  TestCompositeIdObjectType::Entry mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_test::TestCompositeIdObjectType> {
public:
  size_t operator()(const holgen_blackbox_test::TestCompositeIdObjectType &obj) const {
    return std::hash<holgen_blackbox_test::TestCompositeIdObjectType::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestCompositeIdObjectType::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestCompositeIdObjectType::Entry &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", holgen_blackbox_test::TestCompositeIdObjectType(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestCompositeIdObjectType> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestCompositeIdObjectType &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
#define holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType \
    holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType_DOER(Human) \
    holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType_DOER(Cat) \
    holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType_DOER(Dog)
#define holgen_blackbox_test_SWITCH_TestCompositeIdObjectType(VALUE) \
    switch ((VALUE)) { \
    case holgen_blackbox_test::TestCompositeIdObjectType::Human: \
      holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType_DOER(Human) \
    case holgen_blackbox_test::TestCompositeIdObjectType::Cat: \
      holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType_DOER(Cat) \
    case holgen_blackbox_test::TestCompositeIdObjectType::Dog: \
      holgen_blackbox_test_FOR_EACH_TestCompositeIdObjectType_DOER(Dog) \
    }
