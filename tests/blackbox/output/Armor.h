#pragma once

#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Armor {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  const std::string& GetName() const;
  std::string& GetName();
  void SetName(const std::string& val);
  const std::string& GetAlternativeName() const;
  std::string& GetAlternativeName();
  void SetAlternativeName(const std::string& val);
  int8_t GetArmorClass() const;
  void SetArmorClass(int8_t val);
  static Armor* Get(uint32_t id);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  std::string mAlternativeName;
  int8_t mArmorClass;
};
}
