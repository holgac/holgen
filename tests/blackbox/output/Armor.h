#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Armor {
public:
  uint32_t GetId() const;
  const std::string& GetName() const;
  std::string& GetName();
  const std::string& GetAlternativeName() const;
  std::string& GetAlternativeName();
  int8_t GetArmorClass() const;
  void SetId(uint32_t val);
  void SetName(const std::string& val);
  void SetAlternativeName(const std::string& val);
  void SetArmorClass(int8_t val);
  static Armor* Get(uint32_t id);
  static Armor* GetFromName(const std::string& val);
  static Armor* GetFromAlternativeName(const std::string& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  void PushGlobalToLua(lua_State* luaState, const char* name) const;
  static Armor* ReadFromLua(lua_State* luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  std::string mAlternativeName;
  int8_t mArmorClass;
};
}
