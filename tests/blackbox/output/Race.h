#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Race {
public:
  uint32_t GetId() const;
  const std::string& GetName() const;
  std::string& GetName();
  const std::vector<std::string>& GetHairColors() const;
  std::vector<std::string>& GetHairColors();
  const std::map<std::string, std::vector<std::string>>& GetNames() const;
  std::map<std::string, std::vector<std::string>>& GetNames();
  void SetId(uint32_t val);
  void SetName(const std::string& val);
  void SetHairColors(const std::vector<std::string>& val);
  void SetNames(const std::map<std::string, std::vector<std::string>>& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static Race* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  std::vector<std::string> mHairColors;
  std::map<std::string, std::vector<std::string>> mNames;
};
}
