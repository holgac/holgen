// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <lua.hpp>

namespace ex1_schemas {
class LuaHelper {
public:
  template <typename T>
  static void Push(const T &data, lua_State *luaState) {
    if constexpr(std::is_pointer_v<T>) {
      if (data) {
        LuaHelper::Push(*data, luaState);
      } else {
        lua_pushnil(luaState);
      }
    } else {
      data.PushToLua(luaState);
    }
  }
  static void Push(std::nullptr_t , lua_State *luaState);
  static void Push(bool data, lua_State *luaState);
  static void Push(double data, lua_State *luaState);
  static void Push(float data, lua_State *luaState);
  static void Push(int16_t data, lua_State *luaState);
  static void Push(int32_t data, lua_State *luaState);
  static void Push(int64_t data, lua_State *luaState);
  static void Push(int8_t data, lua_State *luaState);
  static void Push(const std::string &data, lua_State *luaState);
  static void Push(uint16_t data, lua_State *luaState);
  static void Push(uint32_t data, lua_State *luaState);
  static void Push(uint64_t data, lua_State *luaState);
  static void Push(uint8_t data, lua_State *luaState);
  template <typename T, size_t C>
  static void Push(std::array<T, C> &data, lua_State *luaState) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(std::deque<T> &data, lua_State *luaState) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(std::vector<T> &data, lua_State *luaState) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(std::set<T> &data, lua_State *luaState) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename T>
  static void Push(std::unordered_set<T> &data, lua_State *luaState) {
    lua_newtable(luaState);
    int index = 0;
    for (auto& elem: data) {
      Push(elem, luaState);
      lua_rawseti(luaState, -2, index++);
    }
  }
  template <typename K, typename V>
  static void Push(std::map<K, V> &data, lua_State *luaState) {
    lua_newtable(luaState);
    for (auto& [key, value]: data) {
      Push(key, luaState);
      Push(value, luaState);
      lua_settable(luaState, -3);
    }
  }
  template <typename K, typename V>
  static void Push(std::unordered_map<K, V> &data, lua_State *luaState) {
    lua_newtable(luaState);
    for (auto& [key, value]: data) {
      Push(key, luaState);
      Push(value, luaState);
      lua_settable(luaState, -3);
    }
  }
  template <typename T>
  static bool Read(T &data, lua_State *luaState, int32_t luaIndex) {
    // *data = T::ReadFromLua(luaState, luaIndex);
    return false; //*data != nullptr;
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
  template <typename T, size_t C>
  static bool Read(const std::array<T, C> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename T>
  static bool Read(const std::deque<T> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename T>
  static bool Read(const std::vector<T> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename T>
  static bool Read(const std::set<T> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename T>
  static bool Read(const std::unordered_set<T> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename K, typename V>
  static bool Read(const std::map<K, V> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  template <typename K, typename V>
  static bool Read(const std::unordered_map<K, V> &data, lua_State *luaState, int32_t luaIndex) {
    return false;
  }
  static void CreateMetatables(lua_State *luaState);
};
}
