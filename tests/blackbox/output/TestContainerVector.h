#pragma once

#include "holgen.h"
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <rapidjson/fwd.h>
#include "TestContainerInnerStructWithId.h"
#include "TestContainerInnerStructNoId.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestContainerVector {
public:
  const std::vector<TestContainerInnerStructWithId>& GetInnerStructsWithId() const;
  std::vector<TestContainerInnerStructWithId>& GetInnerStructsWithId();
  const std::vector<TestContainerInnerStructNoId>& GetInnerStructsNoId() const;
  std::vector<TestContainerInnerStructNoId>& GetInnerStructsNoId();
  const std::vector<std::string>& GetStringContainer() const;
  std::vector<std::string>& GetStringContainer();
  const std::vector<uint32_t>& GetUnsignedContainer() const;
  std::vector<uint32_t>& GetUnsignedContainer();
  void SetInnerStructsWithId(const std::vector<TestContainerInnerStructWithId>& val);
  void SetInnerStructsNoId(const std::vector<TestContainerInnerStructNoId>& val);
  void SetStringContainer(const std::vector<std::string>& val);
  void SetUnsignedContainer(const std::vector<uint32_t>& val);
  const TestContainerInnerStructWithId* GetInnerStructWithIdFromName(const std::string& key) const;
  TestContainerInnerStructWithId* GetInnerStructWithIdFromName(const std::string& key);
  bool AddInnerStructWithId(TestContainerInnerStructWithId&& elem);
  bool AddInnerStructWithId(TestContainerInnerStructWithId& elem);
  const TestContainerInnerStructWithId* GetInnerStructWithId(uint32_t idx) const;
  TestContainerInnerStructWithId* GetInnerStructWithId(uint32_t idx);
  size_t GetInnerStructWithIdCount() const;
  const TestContainerInnerStructNoId* GetInnerStructNoIdFromName(const std::string& key) const;
  TestContainerInnerStructNoId* GetInnerStructNoIdFromName(const std::string& key);
  bool AddInnerStructNoId(TestContainerInnerStructNoId&& elem);
  bool AddInnerStructNoId(const TestContainerInnerStructNoId& elem);
  const TestContainerInnerStructNoId* GetInnerStructNoId(size_t idx) const;
  TestContainerInnerStructNoId* GetInnerStructNoId(size_t idx);
  void DeleteInnerStructNoId(size_t idx);
  size_t GetInnerStructNoIdCount() const;
  bool AddStringElem(std::string&& elem);
  bool AddStringElem(const std::string& elem);
  const std::string* GetStringElem(size_t idx) const;
  std::string* GetStringElem(size_t idx);
  void DeleteStringElem(size_t idx);
  size_t GetStringElemCount() const;
  bool AddUnsignedElem(uint32_t&& elem);
  bool AddUnsignedElem(uint32_t elem);
  const uint32_t* GetUnsignedElem(size_t idx) const;
  uint32_t* GetUnsignedElem(size_t idx);
  void DeleteUnsignedElem(size_t idx);
  size_t GetUnsignedElemCount() const;
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static TestContainerVector* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  static void PushIndexMetaMethod(lua_State* luaState);
  static void PushNewIndexMetaMethod(lua_State* luaState);
  std::vector<TestContainerInnerStructWithId> mInnerStructsWithId;
  std::vector<TestContainerInnerStructNoId> mInnerStructsNoId;
  std::vector<std::string> mStringContainer;
  std::vector<uint32_t> mUnsignedContainer;
  std::map<std::string, uint32_t> mInnerStructsWithIdNameIndex;
  std::map<std::string, size_t> mInnerStructsNoIdNameIndex;
};
}
