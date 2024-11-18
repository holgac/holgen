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
// Defined in tests/blackbox/schemas/testvariant.hsc
class TestVariantStructType {
public:
  using UnderlyingType=uint8_t;
  enum Entry : UnderlyingType {
    Human = 0,
    Cat = 1,
  };
  explicit TestVariantStructType(UnderlyingType value = Invalid);
  TestVariantStructType(Entry value);
  TestVariantStructType::Entry GetValue() const;
  static TestVariantStructType FromString(std::string_view str);
  const char *ToString() const;
  TestVariantStructType &operator=(UnderlyingType rhs);
  bool operator==(UnderlyingType rhs) const;
  bool operator==(const TestVariantStructType &rhs) const;
  bool operator!=(UnderlyingType rhs) const;
  bool operator!=(const TestVariantStructType &rhs) const;
  bool operator<(UnderlyingType rhs) const;
  bool operator<(const TestVariantStructType &rhs) const;
  constexpr static std::array<TestVariantStructType::Entry, 2> GetEntries() {
    return std::array<TestVariantStructType::Entry, 2>{Human, Cat};
  }
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestVariantStructType ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void PushEnumToLua(lua_State *luaState);
  static constexpr const UnderlyingType Invalid = 2;
private:
  TestVariantStructType::Entry mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_test::TestVariantStructType> {
public:
  size_t operator()(const holgen_blackbox_test::TestVariantStructType &obj) const {
    return std::hash<holgen_blackbox_test::TestVariantStructType::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestVariantStructType::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestVariantStructType::Entry &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", holgen_blackbox_test::TestVariantStructType(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestVariantStructType> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestVariantStructType &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
#define holgen_blackbox_test_FOR_EACH_TestVariantStructType \
    holgen_blackbox_test_FOR_EACH_TestVariantStructType_DOER(Human) \
    holgen_blackbox_test_FOR_EACH_TestVariantStructType_DOER(Cat)
#define holgen_blackbox_test_SWITCH_TestVariantStructType(VALUE) \
    switch ((VALUE)) { \
    case holgen_blackbox_test::TestVariantStructType::Human: \
      holgen_blackbox_test_FOR_EACH_TestVariantStructType_DOER(Human) \
    case holgen_blackbox_test::TestVariantStructType::Cat: \
      holgen_blackbox_test_FOR_EACH_TestVariantStructType_DOER(Cat) \
    }
