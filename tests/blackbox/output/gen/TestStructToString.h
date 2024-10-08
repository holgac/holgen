// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <format>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestStructToString {
public:
  TestStructToString() = default;
  TestStructToString(const TestStructToString &rhs) = default;
  TestStructToString(TestStructToString &&rhs) = default;
  ~TestStructToString() = default;
  bool operator==(const TestStructToString &rhs) const;
  const std::string &GetField1() const;
  std::string &GetField1();
  const std::string &GetField2() const;
  std::string &GetField2();
  void SetField1(const std::string &val);
  void SetField2(const std::string &val);
  std::string Stringify() const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructToString *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructToString ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructToString &operator=(const TestStructToString &rhs) = default;
  TestStructToString &operator=(TestStructToString &&rhs) = default;
  inline static const char *CLASS_NAME = "TestStructToString";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int StringifyCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::string mField1;
  std::string mField2;
};
}
namespace std {
template <>
struct formatter<holgen_blackbox_test::TestStructToString> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const holgen_blackbox_test::TestStructToString &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", obj.Stringify());
  }
};
}
