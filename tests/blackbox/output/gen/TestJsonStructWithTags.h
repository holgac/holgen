// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <vector>
#include <cstdint>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestJsonStructWithTags {
public:
  bool operator==(const TestJsonStructWithTags& rhs) const;
  const std::vector<uint64_t>& GetTags() const;
  std::vector<uint64_t>& GetTags();
  void SetTags(const std::vector<uint64_t>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  // This only works with negative indices
  static TestJsonStructWithTags* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
private:
  static int IndexMetaMethod(lua_State* luaState);
  static int NewIndexMetaMethod(lua_State* luaState);
  std::vector<uint64_t> mTags;
};
}
