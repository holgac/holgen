#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Boot.h"
#include "Armor.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Character {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  const std::string& GetName() const;
  std::string& GetName();
  void SetName(const std::string& val);
  const Boot* GetBoot() const;
  Boot* GetBoot();
  uint32_t GetBootId() const;
  void SetBootId(uint32_t val);
  const Armor* GetArmor() const;
  Armor* GetArmor();
  uint32_t GetArmorId() const;
  void SetArmorId(uint32_t val);
  static Character* Get(uint32_t id);
  static Character* GetFromName(const std::string& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  uint32_t mBootId = -1;
  uint32_t mArmorId = -1;
};
}
