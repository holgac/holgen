#pragma once

#include "holgen.h"
#include <string>
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
  UnderlyingType Get() const;
  static TestEnum FromString(std::string_view str);
  std::string ToString() const;
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
  inline static constexpr const UnderlyingType Entry5Value = 5;
  static const TestEnum Entry5;
  inline static constexpr const UnderlyingType Entry1Value = 0;
  static const TestEnum Entry1;
  inline static constexpr const UnderlyingType Entry2Value = 1;
  static const TestEnum Entry2;
  inline static constexpr const UnderlyingType Invalid = 2;
protected:
private:
  UnderlyingType mValue;
};
}
