// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <format>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestEnum {
public:
  using UnderlyingType=uint8_t;
  enum Entry : UnderlyingType {
    Entry5 = 5,
    Entry1 = 0,
    Entry2 = 1,
  };
  explicit TestEnum(UnderlyingType value = Invalid);
  TestEnum(Entry value);
  TestEnum::Entry GetValue() const;
  static TestEnum FromString(std::string_view str);
  const char *ToString() const;
  TestEnum &operator=(UnderlyingType rhs);
  bool operator==(UnderlyingType rhs) const;
  bool operator==(const TestEnum &rhs) const;
  bool operator!=(UnderlyingType rhs) const;
  bool operator!=(const TestEnum &rhs) const;
  bool operator<(UnderlyingType rhs) const;
  bool operator<(const TestEnum &rhs) const;
  constexpr static std::array<TestEnum::Entry, 3> GetEntries() {
    return std::array<TestEnum::Entry, 3>{Entry5, Entry1, Entry2};
  }
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestEnum ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void PushEnumToLua(lua_State *luaState);
  inline static constexpr const UnderlyingType Invalid = 2;
private:
  TestEnum::Entry mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_test::TestEnum> {
public:
  size_t operator()(const holgen_blackbox_test::TestEnum &obj) const {
    return std::hash<holgen_blackbox_test::TestEnum::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestEnum::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestEnum::Entry &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", holgen_blackbox_test::TestEnum(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestEnum> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestEnum &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
