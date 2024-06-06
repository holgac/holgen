// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestBitmap.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestBitmapStruct {
public:
  bool operator==(const TestBitmapStruct &rhs) const;
  const TestBitmap &GetBitmapField() const;
  TestBitmap &GetBitmapField();
  void SetBitmapField(const TestBitmap &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static TestBitmapStruct *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  TestBitmap mBitmapField;
};
}
