#include "GridCell.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
int32_t GridCell::GetX() const {
  return mX;
}
int32_t GridCell::GetY() const {
  return mY;
}
const LandscapeType& GridCell::GetLandscape() const {
  return mLandscape;
}
LandscapeType& GridCell::GetLandscape() {
  return mLandscape;
}
void GridCell::SetX(int32_t val) {
  mX = val;
}
void GridCell::SetY(int32_t val) {
  mY = val;
}
void GridCell::SetLandscape(const LandscapeType& val) {
  mLandscape = val;
}
bool GridCell::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "x")) {
      auto res = JsonHelper::Parse(mX, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "y")) {
      auto res = JsonHelper::Parse(mY, data.value, converter);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "landscape")) {
      auto res = JsonHelper::Parse(mLandscape, data.value, converter);
      if (!res)
        return false;
    }
  }
  return true;
}
void GridCell::PushToLua(lua_State* luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void*)this);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "GridCellMeta");
  lua_setmetatable(luaState, -2);
}
void GridCell::CreateLuaMetatable(lua_State* luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -3);
    auto instance = (GridCell*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -2);
    if (0 == strcmp("x", key)) {
      LuaHelper::Push(instance->mX, ls);
    } else if (0 == strcmp("y", key)) {
      LuaHelper::Push(instance->mY, ls);
    } else if (0 == strcmp("landscape", key)) {
      LuaHelper::Push(instance->mLandscape, ls);
    } else {
      return 0;
    }
    return 1;
  });
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, [](lua_State* ls) {
    lua_pushstring(ls, "p");
    lua_gettable(ls, -4);
    auto instance = (GridCell*)lua_touserdata(ls, -1);
    const char* key = lua_tostring(ls, -3);
    if (0 == strcmp("x", key)) {
      LuaHelper::Read(instance->mX, ls, -2);
    } else if (0 == strcmp("y", key)) {
      LuaHelper::Read(instance->mY, ls, -2);
    } else if (0 == strcmp("landscape", key)) {
      LuaHelper::Read(instance->mLandscape, ls, -2);
    }
    return 0;
  });
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "GridCellMeta");
}
}
