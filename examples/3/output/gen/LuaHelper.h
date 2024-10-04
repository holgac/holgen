// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <lua.hpp>

namespace ex3_schemas {
class LuaHelper {
public:
  template <typename T>
  static void Push(const T &data, lua_State *luaState, bool pushMirror) {
    if constexpr(std::is_pointer_v<T>) {
      if (data) {
        LuaHelper::Push(*data, luaState, pushMirror);
      } else {
        lua_pushnil(luaState);
      }
    } else if (pushMirror) {
      data.PushMirrorToLua(luaState);
    } else {
      data.PushToLua(luaState);
    }
  }
  static void Push(std::nullptr_t , lua_State *luaState, bool pushMirror);
  static void Push(bool data, lua_State *luaState, bool pushMirror);
  static void Push(double data, lua_State *luaState, bool pushMirror);
  static void Push(float data, lua_State *luaState, bool pushMirror);
  static void Push(int16_t data, lua_State *luaState, bool pushMirror);
  static void Push(int32_t data, lua_State *luaState, bool pushMirror);
  static void Push(int64_t data, lua_State *luaState, bool pushMirror);
  static void Push(int8_t data, lua_State *luaState, bool pushMirror);
  static void Push(const std::string &data, lua_State *luaState, bool pushMirror);
  static void Push(uint16_t data, lua_State *luaState, bool pushMirror);
  static void Push(uint32_t data, lua_State *luaState, bool pushMirror);
  static void Push(uint64_t data, lua_State *luaState, bool pushMirror);
  static void Push(uint8_t data, lua_State *luaState, bool pushMirror);
  template <typename T, size_t C>
  static void Push(const std::array<T, C> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState, pushMirror);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(const std::deque<T> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState, pushMirror);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(const std::vector<T> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState, pushMirror);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(const std::list<T> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState, pushMirror);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(const std::set<T> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState, pushMirror);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(const std::unordered_set<T> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState, pushMirror);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename K, typename V>
  static void Push(const std::map<K, V> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    for (auto& [key, value]: data) {
      Push(key, luaState, pushMirror);
      Push(value, luaState, pushMirror);
      lua_settable(luaState, -3);
    }
  }
  template <typename K, typename V>
  static void Push(const std::unordered_map<K, V> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    for (auto& [key, value]: data) {
      Push(key, luaState, pushMirror);
      Push(value, luaState, pushMirror);
      lua_settable(luaState, -3);
    }
  }
  template <typename T0, typename T1>
  static void Push(const std::pair<T0, T1> &data, lua_State *luaState, bool pushMirror) {
    lua_newtable(luaState);
    Push(std::get<0>(data), luaState, pushMirror);
    lua_rawseti(luaState, -2, 0);
    Push(std::get<1>(data), luaState, pushMirror);
    lua_rawseti(luaState, -2, 1);
  }
  template <typename T>
  static bool Read(T &data, lua_State *luaState, int32_t luaIndex) {
    data = T::ReadMirrorFromLua(luaState, luaIndex);
    return true;
  }
  static bool Read(bool &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(double &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(float &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(int16_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(int32_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(int64_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(int8_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(std::string &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(uint16_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(uint32_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(uint64_t &data, lua_State *luaState, int32_t luaIndex);
  static bool Read(uint8_t &data, lua_State *luaState, int32_t luaIndex);
  /*
   * BEWARE: This overload assumes that the function will be called before the entry in stack is invalidated.
   * It was made specifically for forwarding lua parameters back to lua; do not use it for anything else.
   */
  static bool Read(std::function<void(lua_State *)> &data, lua_State *luaState, int32_t luaIndex);
  template <typename T, size_t C>
  static bool Read(std::array<T, C> &data, lua_State *luaState, int32_t luaIndex) {
    size_t nextIdx = 0;
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    while (lua_next(luaState, -2)) {
      bool res = Read(data[nextIdx], luaState, -1);
      ++nextIdx;
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not read data from lua into a container");
      lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);
    return true;
  }
  template <typename T>
  static bool Read(std::deque<T> &data, lua_State *luaState, int32_t luaIndex) {
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    while (lua_next(luaState, -2)) {
      bool res = Read(data.emplace_back(), luaState, -1);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not read data from lua into a container");
      lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);
    return true;
  }
  template <typename T>
  static bool Read(std::vector<T> &data, lua_State *luaState, int32_t luaIndex) {
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    while (lua_next(luaState, -2)) {
      bool res = Read(data.emplace_back(), luaState, -1);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not read data from lua into a container");
      lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);
    return true;
  }
  template <typename T>
  static bool Read(std::list<T> &data, lua_State *luaState, int32_t luaIndex) {
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    while (lua_next(luaState, -2)) {
      bool res = Read(data.emplace_back(), luaState, -1);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not read data from lua into a container");
      lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);
    return true;
  }
  template <typename T>
  static bool Read(std::set<T> &data, lua_State *luaState, int32_t luaIndex) {
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    while (lua_next(luaState, -2)) {
      T elem;
      bool res = Read(elem, luaState, -1);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not read data from lua into a container");
      data.insert(elem);
      lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);
    return true;
  }
  template <typename T>
  static bool Read(std::unordered_set<T> &data, lua_State *luaState, int32_t luaIndex) {
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    while (lua_next(luaState, -2)) {
      T elem;
      bool res = Read(elem, luaState, -1);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not read data from lua into a container");
      data.insert(elem);
      lua_pop(luaState, 1);
    }
    lua_pop(luaState, 1);
    return true;
  }
  template <typename K, typename V>
  static bool Read(std::map<K, V> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename K, typename V>
  static bool Read(std::unordered_map<K, V> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename T0, typename T1>
  static bool Read(std::pair<T0, T1> &data, lua_State *luaState, int32_t luaIndex) {
    lua_newtable(luaState);
    lua_pushvalue(luaState, luaIndex);
    lua_pushnil(luaState);
    HOLGEN_WARN_AND_RETURN_IF(!lua_next(luaState, -2), false, "Exhausted elements when reading from lua into a std::pair");
    Read(std::get<0>(data), luaState, -1);
    lua_pop(luaState, 1);
    HOLGEN_WARN_AND_RETURN_IF(!lua_next(luaState, -2), false, "Exhausted elements when reading from lua into a std::pair");
    Read(std::get<1>(data), luaState, -1);
    lua_pop(luaState, 1);
    HOLGEN_WARN_AND_RETURN_IF(lua_next(luaState, -2), false, "Too many elements when from lua into a std::pair");
    lua_pop(luaState, 1);
    return true;
  }
  static void CreateMetatables(lua_State *luaState);
};
}
