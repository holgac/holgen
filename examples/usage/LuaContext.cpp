#include "LuaContext.h"
#include <lua.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <format>

namespace holgen::usages {

namespace {
std::string ElemToString(lua_State *state, int idx) {
  auto luaType = lua_type(state, idx);
  switch (luaType) {
  case LUA_TNIL:
    return "null";
  case LUA_TBOOLEAN:
    return lua_toboolean(state, idx) ? "true" : "false";
  case LUA_TLIGHTUSERDATA:
    return "lightuserdata";
  case LUA_TNUMBER:
    return std::format("{}", lua_tonumber(state, idx));
  case LUA_TSTRING:
    return lua_tostring(state, idx);
  case LUA_TTABLE: {
    // necessary if idx < 0 (because pushing changes negative indices)
    lua_pushvalue(state, idx);
    lua_pushnil(state);
    std::stringstream ss;
    ss << "{";
    bool isFirst = true;
    while (lua_next(state, -2)) {
      if (isFirst) {
        isFirst = false;
      } else {
        ss << ",";
      }
      auto value = ElemToString(state, -1);
      lua_pop(state, 1);
      auto key = ElemToString(state, -1);
      ss << key << ":" << value;
    }
    // Pop the table we reinserted
    lua_pop(state, 1);
    ss << "}";
    return ss.str();
  }
  case LUA_TFUNCTION:
    return "function";
  case LUA_TUSERDATA:
    return "userdata";
  case LUA_TTHREAD:
    return "thread";
  default:
    return std::format("unknown: {}", luaType);
  }
}
} // namespace armonium::core::lua

LuaContext::LuaContext(lua_State *state) :
  mOwn(false) {
  mState = state;
}

LuaContext::LuaContext(LuaContext::CreateValue) :
  mOwn(true) {
  mState = luaL_newstate();
  luaL_openlibs(mState);
}

LuaContext::~LuaContext() {
  if (mOwn) {
    lua_close(mState);
  }
}

std::string LuaContext::Execute(const std::string &str) {
  auto initialTop = lua_gettop(mState);
  luaL_dostring(mState, str.c_str());
  std::stringstream ss;
  bool isFirst = true;
  auto newTop = lua_gettop(mState);
  for (int i = initialTop + 1; i <= newTop; ++i) {
    if (isFirst)
      isFirst = false;
    else
      ss << ", ";
    ss << ElemToString(mState, i);
  }
  if (newTop > initialTop)
    lua_pop(mState, newTop - initialTop);
  return ss.str();
}

std::string LuaContext::ExecuteReturn(const std::string &str) {
  return Execute("return " + str);
}

LuaContext::operator lua_State *() {
  return mState;
}

std::string LuaContext::ExecuteFile(const std::filesystem::path &path) {
  std::ifstream fin(path, std::ios_base::binary);
  fin.seekg(0, std::ios_base::end);
  auto bufferSize = fin.tellg();
  bufferSize += 1;
  std::string contents(bufferSize, 0);
  fin.seekg(0, std::ios_base::beg);
  fin.read(contents.data(), contents.size());
  return Execute(contents);
}
} // namespace armonium::core::lua
