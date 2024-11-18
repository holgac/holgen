// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <vector>
#include <rapidjson/fwd.h>
#include "TestStructSingleElem.h"
#include "TestStructSingleElemWithId.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructSingleElemContainer {
public:
  TestStructSingleElemContainer() = default;
  TestStructSingleElemContainer(const TestStructSingleElemContainer &rhs) = default;
  TestStructSingleElemContainer(TestStructSingleElemContainer &&rhs) noexcept = default;
  ~TestStructSingleElemContainer() = default;
  const std::vector<TestStructSingleElem> &GetSingleElemStructs() const;
  std::vector<TestStructSingleElem> &GetSingleElemStructs();
  const std::vector<TestStructSingleElemWithId> &GetSingleElemStructsWithId() const;
  std::vector<TestStructSingleElemWithId> &GetSingleElemStructsWithId();
  void SetSingleElemStructs(const std::vector<TestStructSingleElem> &val);
  void SetSingleElemStructsWithId(const std::vector<TestStructSingleElemWithId> &val);
  TestStructSingleElem *AddSingleElemStruct(TestStructSingleElem &&elem);
  TestStructSingleElem *AddSingleElemStruct(const TestStructSingleElem &elem);
  const TestStructSingleElem *GetSingleElemStruct(size_t idx) const;
  TestStructSingleElem *GetSingleElemStruct(size_t idx);
  void DeleteSingleElemStruct(size_t idx);
  size_t GetSingleElemStructCount() const;
  TestStructSingleElemWithId *AddSingleElemStructWithId(TestStructSingleElemWithId &&elem);
  TestStructSingleElemWithId *AddSingleElemStructWithId(TestStructSingleElemWithId &elem);
  const TestStructSingleElemWithId *GetSingleElemStructWithId(uint32_t idx) const;
  TestStructSingleElemWithId *GetSingleElemStructWithId(uint32_t idx);
  size_t GetSingleElemStructWithIdCount() const;
  bool operator==(const TestStructSingleElemContainer &rhs) const;
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  rapidjson::Value DumpJson(rapidjson::Document &doc) const;
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructSingleElemContainer *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static TestStructSingleElemContainer ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructSingleElemContainer &operator=(const TestStructSingleElemContainer &rhs) = default;
  TestStructSingleElemContainer &operator=(TestStructSingleElemContainer &&rhs) noexcept = default;
  inline static const char *CLASS_NAME = "TestStructSingleElemContainer";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int AddSingleElemStructCallerFromLua(lua_State *luaState);
  static int GetSingleElemStructCallerFromLua(lua_State *luaState);
  static int DeleteSingleElemStructCallerFromLua(lua_State *luaState);
  static int GetSingleElemStructCountCallerFromLua(lua_State *luaState);
  static int AddSingleElemStructWithIdCallerFromLua(lua_State *luaState);
  static int GetSingleElemStructWithIdCallerFromLua(lua_State *luaState);
  static int GetSingleElemStructWithIdCountCallerFromLua(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::vector<TestStructSingleElem> mSingleElemStructs;
  std::vector<TestStructSingleElemWithId> mSingleElemStructsWithId;
};
}
