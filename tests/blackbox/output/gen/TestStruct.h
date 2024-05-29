// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestStruct {
public:
  bool operator==(const TestStruct &rhs) const;
  bool GetTestFieldBool() const;
  uint32_t GetTestFieldUnsigned() const;
  const std::string &GetTestFieldString() const;
  std::string &GetTestFieldString();
  template <typename T>
  const T *GetTestFieldUserdata() const {
    return reinterpret_cast<const T*>(mTestFieldUserdata);
  }
  template <typename T>
  T *GetTestFieldUserdata() {
    return reinterpret_cast<T*>(mTestFieldUserdata);
  }
  void SetTestFieldBool(bool val);
  void SetTestFieldUnsigned(uint32_t val);
  void SetTestFieldString(const std::string &val);
  template <typename T>
  void SetTestFieldUserdata(T *val) {
    mTestFieldUserdata = reinterpret_cast<void*>(val);
  }
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static TestStruct *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  bool mTestFieldBool;
  uint32_t mTestFieldUnsigned = 42;
  std::string mTestFieldString;
  void *mTestFieldUserdata = nullptr;
};
}
