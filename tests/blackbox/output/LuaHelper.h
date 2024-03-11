#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <deque>
#include <vector>
#include <map>
#include <unordered_map>
#include <lua.hpp>

namespace holgen_blackbox_test {
class LuaHelper {
public:
  template <typename T>
  static void Push(const T& data, lua_State* luaState) {
    data.PushToLua(luaState);
  }
  static void Push(nullptr_t , lua_State* luaState);
  template <typename T>
  static void Push(const T* ptr, lua_State* luaState) {
    if (ptr) {
      ptr->PushToLua(luaState);
    } else {
      lua_pushnil(luaState);
    }
  }
  static void Push(bool data, lua_State* luaState);
  static void Push(double data, lua_State* luaState);
  static void Push(float data, lua_State* luaState);
  static void Push(int16_t data, lua_State* luaState);
  static void Push(int32_t data, lua_State* luaState);
  static void Push(int64_t data, lua_State* luaState);
  static void Push(int8_t data, lua_State* luaState);
  static void Push(const std::string& data, lua_State* luaState);
  static void Push(uint16_t data, lua_State* luaState);
  static void Push(uint32_t data, lua_State* luaState);
  static void Push(uint64_t data, lua_State* luaState);
  static void Push(uint8_t data, lua_State* luaState);
  template <typename T>
  static void Push(std::deque<T>& data, lua_State* luaState) {
  }
  template <typename T>
  static void Push(std::vector<T>& data, lua_State* luaState) {
  }
  template <typename K, typename V>
  static void Push(std::map<K, V>& data, lua_State* luaState) {
  }
  template <typename K, typename V>
  static void Push(std::unordered_map<K, V>& data, lua_State* luaState) {
  }
  template <typename T>
  static bool Read(T& data, lua_State* luaState, int32_t luaIndex) {
    // *data = T::ReadFromLua(luaState, luaIndex);
    return false; //*data != nullptr;
  }
  static bool Read(bool& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(double& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(float& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(int16_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(int32_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(int64_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(int8_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(std::string& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(uint16_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(uint32_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(uint64_t& data, lua_State* luaState, int32_t luaIndex);
  static bool Read(uint8_t& data, lua_State* luaState, int32_t luaIndex);
  template <typename T>
  static bool Read(const std::deque<T>& data, lua_State* luaState, int32_t luaIndex) {
    return false;
  }
  template <typename T>
  static bool Read(const std::vector<T>& data, lua_State* luaState, int32_t luaIndex) {
    return false;
  }
  template <typename K, typename V>
  static bool Read(const std::map<K, V>& data, lua_State* luaState, int32_t luaIndex) {
    return false;
  }
  template <typename K, typename V>
  static bool Read(const std::unordered_map<K, V>& data, lua_State* luaState, int32_t luaIndex) {
    return false;
  }
protected:
private:
};
}
