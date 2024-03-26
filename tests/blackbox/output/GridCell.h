#pragma once

#include "holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "LandscapeType.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class GridCell {
public:
  int32_t GetX() const;
  int32_t GetY() const;
  const LandscapeType& GetLandscape() const;
  LandscapeType& GetLandscape();
  void SetX(int32_t val);
  void SetY(int32_t val);
  void SetLandscape(const LandscapeType& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static GridCell* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  static void PushIndexMetaMethod(lua_State* luaState);
  static void PushNewIndexMetaMethod(lua_State* luaState);
  int32_t mX;
  int32_t mY;
  LandscapeType mLandscape;
};
}
