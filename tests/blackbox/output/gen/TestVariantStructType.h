// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <format>
#include <array>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestVariantStructType.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestVariantStructType {
public:
  using UnderlyingType=int64_t;
  explicit TestVariantStructType(UnderlyingType value = Invalid);
  UnderlyingType GetValue() const;
  static TestVariantStructType FromString(std::string_view str);
  const char* ToString() const;
  TestVariantStructType& operator =(const TestVariantStructType& rhs);
  TestVariantStructType& operator =(UnderlyingType rhs);
  bool operator ==(UnderlyingType rhs) const;
  bool operator ==(const TestVariantStructType& rhs) const;
  bool operator !=(UnderlyingType rhs) const;
  bool operator !=(const TestVariantStructType& rhs) const;
  bool operator <(UnderlyingType rhs) const;
  bool operator <(const TestVariantStructType& rhs) const;
  constexpr static std::array<TestVariantStructType::UnderlyingType, 2> GetEntryValues() {
    return std::array<TestVariantStructType::UnderlyingType, 2>{HumanValue, CatValue};
  }
  static std::array<TestVariantStructType, 2> GetEntries();
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static const TestVariantStructType Human;
  static const TestVariantStructType Cat;
  inline static constexpr const UnderlyingType HumanValue = 0;
  inline static constexpr const UnderlyingType CatValue = 1;
  inline static constexpr const UnderlyingType Invalid = 2;
private:
  UnderlyingType mValue;
};
}
namespace std {
template <>
struct hash<holgen_blackbox_test::TestVariantStructType> {
public:
  size_t operator()(const holgen_blackbox_test::TestVariantStructType& obj) const {
    return std::hash<holgen_blackbox_test::TestVariantStructType::UnderlyingType>()(obj.GetValue());
  }
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestVariantStructType> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestVariantStructType& obj, FormatContext& ctx) const {
    return format_to(ctx.out(), "{}", obj.ToString());
  }
};
}
