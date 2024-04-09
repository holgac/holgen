// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestEnum.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestEnum {
public:
  using UnderlyingType=int64_t;
  explicit TestEnum(UnderlyingType value = Invalid);
  UnderlyingType GetValue() const;
  static TestEnum FromString(std::string_view str);
  const char* ToString() const;
  TestEnum& operator =(UnderlyingType rhs);
  bool operator ==(UnderlyingType rhs) const;
  bool operator ==(const TestEnum& rhs) const;
  bool operator !=(UnderlyingType rhs) const;
  bool operator !=(const TestEnum& rhs) const;
  constexpr static std::array<TestEnum::UnderlyingType, 3> GetEntryValues() {
    return std::array<TestEnum::UnderlyingType, 3>{Entry5Value, Entry1Value, Entry2Value};
  }
  static std::array<TestEnum, 3> GetEntries();
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static const TestEnum Entry5;
  static const TestEnum Entry1;
  static const TestEnum Entry2;
  inline static constexpr const UnderlyingType Entry5Value = 5;
  inline static constexpr const UnderlyingType Entry1Value = 0;
  inline static constexpr const UnderlyingType Entry2Value = 1;
  inline static constexpr const UnderlyingType Invalid = 2;
private:
  UnderlyingType mValue;
};
}
