// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <format>
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestStructArrayType.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestStructArrayType {
public:
  using UnderlyingType=int64_t;
  explicit TestStructArrayType(UnderlyingType value = Invalid);
  UnderlyingType GetValue() const;
  static TestStructArrayType FromString(std::string_view str);
  const char* ToString() const;
  TestStructArrayType& operator =(UnderlyingType rhs);
  bool operator ==(UnderlyingType rhs) const;
  bool operator ==(const TestStructArrayType& rhs) const;
  bool operator !=(UnderlyingType rhs) const;
  bool operator !=(const TestStructArrayType& rhs) const;
  bool operator <(UnderlyingType rhs) const;
  bool operator <(const TestStructArrayType& rhs) const;
  constexpr static std::array<TestStructArrayType::UnderlyingType, 2> GetEntryValues() {
    return std::array<TestStructArrayType::UnderlyingType, 2>{Type1Value, Type2Value};
  }
  static std::array<TestStructArrayType, 2> GetEntries();
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static const TestStructArrayType Type1;
  static const TestStructArrayType Type2;
  inline static constexpr const UnderlyingType Type1Value = 0;
  inline static constexpr const UnderlyingType Type2Value = 1;
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
struct formatter<holgen_blackbox_test::TestStructArrayType> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestStructArrayType& obj, FormatContext& ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
