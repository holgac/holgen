#pragma once

#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
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
  uint32_t GetBoot() const;
  void SetBoot(uint32_t val);
  uint32_t GetArmor() const;
  void SetArmor(uint32_t val);
  static Character* Get(uint32_t id);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  uint32_t mBoot;
  uint32_t mArmor;
};
}
