// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <vector>
#include <map>
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "TestJsonTag.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class TestJsonTagManager {
public:
  bool operator==(const TestJsonTagManager &rhs) const;
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
  bool ParseFiles(const std::string &rootPath, const Converter &converterArg);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  // This only works with negative indices
  static TestJsonTagManager *ReadFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::vector<TestJsonTag> mTags;
  std::map<std::string, uint64_t> mTagsNameIndex;
};
}
