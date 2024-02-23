#pragma once

#include <vector>
#include <cstdint>
#include <rapidjson/document.h>
#include <lua.hpp>
#include "Boot.h"
#include "Armor.h"
#include "Character.h"
#include "Converter.h"

namespace holgen_blackbox_test {
class GameData {
public:
  const std::vector<Boot>& GetBoots() const;
  std::vector<Boot>& GetBoots();
  void SetBoots(const std::vector<Boot>& val);
  void AddBoot(Boot&& elem);
  const Boot& GetBoot(uint32_t idx) const;
  Boot& GetBoot(uint32_t idx);
  const std::vector<Armor>& GetArmors() const;
  std::vector<Armor>& GetArmors();
  void SetArmors(const std::vector<Armor>& val);
  void AddArmor(Armor&& elem);
  const Armor& GetArmor(uint32_t idx) const;
  Armor& GetArmor(uint32_t idx);
  const std::vector<Character>& GetCharacters() const;
  std::vector<Character>& GetCharacters();
  void SetCharacters(const std::vector<Character>& val);
  void AddCharacter(Character&& elem);
  const Character& GetCharacter(uint32_t idx) const;
  Character& GetCharacter(uint32_t idx);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  std::vector<Boot> mBoots;
  std::vector<Armor> mArmors;
  std::vector<Character> mCharacters;
};
}
