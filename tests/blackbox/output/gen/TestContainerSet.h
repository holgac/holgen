// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <set>
#include <string>
#include <rapidjson/fwd.h>

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestContainerSet {
public:
  bool operator==(const TestContainerSet &rhs) const;
  const std::set<std::string> &GetStringContainer() const;
  std::set<std::string> &GetStringContainer();
  const std::set<uint32_t> &GetUnsignedContainer() const;
  std::set<uint32_t> &GetUnsignedContainer();
  void SetStringContainer(const std::set<std::string> &val);
  void SetUnsignedContainer(const std::set<uint32_t> &val);
  const std::string *AddStringElem(std::string &&elem);
  const std::string *AddStringElem(const std::string &elem);
  bool HasStringElem(const std::string &elem) const;
  void DeleteStringElem(std::string elem);
  size_t GetStringElemCount() const;
  const uint32_t *AddUnsignedElem(uint32_t elem);
  bool HasUnsignedElem(uint32_t elem) const;
  void DeleteUnsignedElem(uint32_t elem);
  size_t GetUnsignedElemCount() const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestContainerSet *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestContainerSet ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::set<std::string> mStringContainer;
  std::set<uint32_t> mUnsignedContainer;
};
}
