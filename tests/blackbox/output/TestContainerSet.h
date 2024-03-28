#pragma once

#include "holgen.h"
#include <set>
#include <string>
#include <cstdint>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class TestContainerSet {
public:
  const std::set<std::string>& GetStringContainer() const;
  std::set<std::string>& GetStringContainer();
  const std::set<uint32_t>& GetUnsignedContainer() const;
  std::set<uint32_t>& GetUnsignedContainer();
  void SetStringContainer(const std::set<std::string>& val);
  void SetUnsignedContainer(const std::set<uint32_t>& val);
  bool AddStringElem(std::string&& elem);
  bool AddStringElem(const std::string& elem);
  bool HasStringElem(const std::string& elem) const;
  void DeleteStringElem(std::string elem);
  size_t GetStringElemCount() const;
  bool AddUnsignedElem(uint32_t&& elem);
  bool AddUnsignedElem(const uint32_t& elem);
  bool HasUnsignedElem(uint32_t elem) const;
  void DeleteUnsignedElem(uint32_t elem);
  size_t GetUnsignedElemCount() const;
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static TestContainerSet* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  static void PushIndexMetaMethod(lua_State* luaState);
  static void PushNewIndexMetaMethod(lua_State* luaState);
  std::set<std::string> mStringContainer;
  std::set<uint32_t> mUnsignedContainer;
};
}