// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <rapidjson/fwd.h>
#include "TestJsonTag.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/testjson.hsc
class TestJsonTagManager {
public:
  TestJsonTagManager() = default;
  TestJsonTagManager(const TestJsonTagManager &rhs) = default;
  TestJsonTagManager(TestJsonTagManager &&rhs) noexcept = default;
  ~TestJsonTagManager() = default;
  const std::vector<TestJsonTag> &GetTags() const;
  std::vector<TestJsonTag> &GetTags();
  void SetTags(const std::vector<TestJsonTag> &val);
  uint64_t GetOrInsert(const std::string &tag);
  const TestJsonTag *GetTagFromName(const std::string &key) const;
  TestJsonTag *GetTagFromName(const std::string &key);
  TestJsonTag *AddTag(TestJsonTag &&elem);
  TestJsonTag *AddTag(TestJsonTag &elem);
  const TestJsonTag *GetTag(uint64_t idx) const;
  TestJsonTag *GetTag(uint64_t idx);
  size_t GetTagCount() const;
  bool operator==(const TestJsonTagManager &rhs) const;
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  bool ParseFiles(const std::string &rootPath, const Converter &converterArg);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestJsonTagManager *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestJsonTagManager ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestJsonTagManager &operator=(const TestJsonTagManager &rhs) = default;
  TestJsonTagManager &operator=(TestJsonTagManager &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestJsonTagManager";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int GetOrInsertCallerFromLua(lua_State *luaState);
  static int GetTagFromNameCallerFromLua(lua_State *luaState);
  static int AddTagCallerFromLua(lua_State *luaState);
  static int GetTagCallerFromLua(lua_State *luaState);
  static int GetTagCountCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::vector<TestJsonTag> mTags;
  std::map<std::string, uint64_t> mTagsNameIndex;
};
}
