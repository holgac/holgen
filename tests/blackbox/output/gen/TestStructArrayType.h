// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <format>
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestStructArrayType {
public:
  using UnderlyingType=int64_t;
  enum Entry : UnderlyingType {
    Type1 = 0,
    Type2 = 1,
  };
  explicit TestStructArrayType(UnderlyingType value = Invalid);
  TestStructArrayType(Entry value);
  TestStructArrayType::Entry GetValue() const;
  static TestStructArrayType FromString(std::string_view str);
  const char* ToString() const;
  TestStructArrayType& operator =(UnderlyingType rhs);
  bool operator ==(UnderlyingType rhs) const;
  bool operator ==(const TestStructArrayType& rhs) const;
  bool operator !=(UnderlyingType rhs) const;
  bool operator !=(const TestStructArrayType& rhs) const;
  bool operator <(UnderlyingType rhs) const;
  bool operator <(const TestStructArrayType& rhs) const;
  constexpr static std::array<TestStructArrayType::Entry, 2> GetEntries() {
    return std::array<TestStructArrayType::Entry, 2>{Type1, Type2};
  }
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  // This only works with negative indices
  static TestStructArrayType ReadFromLua(lua_State* luaState, int32_t idx);
  static void PushEnumToLua(lua_State* luaState);
  inline static constexpr const UnderlyingType Invalid = 2;
private:
  UnderlyingType mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_test::TestStructArrayType> {
public:
  size_t operator()(const holgen_blackbox_test::TestStructArrayType& obj) const {
    return std::hash<holgen_blackbox_test::TestStructArrayType::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestStructArrayType::Entry> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestStructArrayType& obj, FormatContext& ctx) const {
    return format_to(ctx.out(), "{}", holgen_blackbox_test::TestStructArrayType(obj).ToString());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestStructArrayType> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestStructArrayType& obj, FormatContext& ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
