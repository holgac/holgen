#pragma once

#include "holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Person {
public:
  uint32_t GetRace() const;
  uint32_t GetCurrentCountry() const;
  uint32_t GetCurrentCity() const;
  uint32_t GetHomeCountry() const;
  uint32_t GetPlaceOfBirth() const;
  void SetRace(uint32_t val);
  void SetCurrentCountry(uint32_t val);
  void SetCurrentCity(uint32_t val);
  void SetHomeCountry(uint32_t val);
  void SetPlaceOfBirth(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mRace;
  uint32_t mCurrentCountry;
  uint32_t mCurrentCity;
  uint32_t mHomeCountry;
  uint32_t mPlaceOfBirth;
};
}
