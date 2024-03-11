#pragma once

#include "holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class RaceId {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static RaceId* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
};
}
