// This file is generated by holgen. Do not modify manually.
#pragma once

#include "holgen.h"
#include <unordered_map>
#include <cstdint>
#include <map>
#include <string>
#include <rapidjson/fwd.h>
#include "Human.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class HumanManager {
public:
  const std::unordered_map<uint32_t, Human>& GetHumans() const;
  std::unordered_map<uint32_t, Human>& GetHumans();
  void SetHumans(const std::unordered_map<uint32_t, Human>& val);
  const Human* GetHumanFromName(const std::string& key) const;
  Human* GetHumanFromName(const std::string& key);
  Human* AddHuman(Human&& elem);
  Human* AddHuman(Human& elem);
  const Human* GetHuman(uint32_t idx) const;
  Human* GetHuman(uint32_t idx);
  bool HasHuman(uint32_t key) const;
  void DeleteHuman(uint32_t key);
  size_t GetHumanCount() const;
  bool ParseFiles(const std::string& rootPath, const Converter& converterArg);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static HumanManager* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  static void PushIndexMetaMethod(lua_State* luaState);
  static void PushNewIndexMetaMethod(lua_State* luaState);
  std::unordered_map<uint32_t, Human> mHumans;
  std::map<std::string, uint32_t> mHumansNameIndex;
  uint32_t mHumansNextId = 0;
};
}
