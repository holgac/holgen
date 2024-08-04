// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestJsonStructMapWithConverters {
public:
  TestJsonStructMapWithConverters() = default;
  TestJsonStructMapWithConverters(const TestJsonStructMapWithConverters &rhs) = default;
  TestJsonStructMapWithConverters(TestJsonStructMapWithConverters &&rhs) = default;
  ~TestJsonStructMapWithConverters() = default;
  bool operator==(const TestJsonStructMapWithConverters &rhs) const;
  const std::map<std::string, uint32_t> &GetTestMapConvertElem() const;
  std::map<std::string, uint32_t> &GetTestMapConvertElem();
  const std::map<uint32_t, std::string> &GetTestMapConvertKey() const;
  std::map<uint32_t, std::string> &GetTestMapConvertKey();
  const std::map<uint32_t, uint32_t> &GetTestMapConvertKeyElem() const;
  std::map<uint32_t, uint32_t> &GetTestMapConvertKeyElem();
  void SetTestMapConvertElem(const std::map<std::string, uint32_t> &val);
  void SetTestMapConvertKey(const std::map<uint32_t, std::string> &val);
  void SetTestMapConvertKeyElem(const std::map<uint32_t, uint32_t> &val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestJsonStructMapWithConverters *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestJsonStructMapWithConverters ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestJsonStructMapWithConverters &operator=(const TestJsonStructMapWithConverters &rhs) = default;
  TestJsonStructMapWithConverters &operator=(TestJsonStructMapWithConverters &&rhs) = default;
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::map<std::string, uint32_t> mTestMapConvertElem;
  std::map<uint32_t, std::string> mTestMapConvertKey;
  std::map<uint32_t, uint32_t> mTestMapConvertKeyElem;
};
}
