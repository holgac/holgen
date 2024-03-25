#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Boot {
public:
  uint32_t GetId() const;
  const std::string& GetName() const;
  std::string& GetName();
  const std::string& GetColor() const;
  std::string& GetColor();
  void SetId(uint32_t val);
  void SetName(const std::string& val);
  void SetColor(const std::string& val);
  static Boot* Get(uint32_t id);
  static Boot* GetFromName(const std::string& key);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static Boot* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  std::string mColor;
};
}
