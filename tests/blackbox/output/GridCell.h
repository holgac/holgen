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
  void SetX(int32_t val);
  int32_t GetY() const;
  void SetY(int32_t val);
  const LandscapeType& GetLandscape() const;
  LandscapeType& GetLandscape();
  void SetLandscape(const LandscapeType& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  int32_t mX;
  int32_t mY;
  LandscapeType mLandscape;
};
}
