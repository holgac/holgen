#pragma once

#include "holgen.h"
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "LandscapeType.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class LandscapeType {
public:
  using UnderlyingType=int64_t;
  explicit LandscapeType(UnderlyingType value = Invalid);
  UnderlyingType Get() const;
  static LandscapeType FromString(std::string_view str);
  std::string ToString() const;
  LandscapeType& operator =(UnderlyingType rhs);
  bool operator ==(UnderlyingType rhs) const;
  bool operator ==(const LandscapeType& rhs) const;
  bool operator !=(UnderlyingType rhs) const;
  bool operator !=(const LandscapeType& rhs) const;
  constexpr static std::array<LandscapeType::UnderlyingType, 4> GetEntryValues();
  static std::array<LandscapeType, 4> GetEntries();
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  inline static constexpr const UnderlyingType LandValue = 0;
  static const LandscapeType Land;
  inline static constexpr const UnderlyingType SeaValue = 1;
  static const LandscapeType Sea;
  inline static constexpr const UnderlyingType RiverValue = 2;
  static const LandscapeType River;
  inline static constexpr const UnderlyingType MountainValue = 3;
  static const LandscapeType Mountain;
  inline static constexpr const UnderlyingType Invalid = 4;
protected:
private:
  UnderlyingType mValue;
};
}
