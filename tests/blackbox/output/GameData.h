#pragma once

#include "holgen.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <map>
#include <rapidjson/fwd.h>
#include "Boot.h"
#include "Armor.h"
#include "Character.h"
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class GameData {
public:
  const std::vector<Boot>& GetBoots() const;
  std::vector<Boot>& GetBoots();
  const std::vector<Armor>& GetArmors() const;
  std::vector<Armor>& GetArmors();
  const std::vector<Character>& GetCharacters() const;
  std::vector<Character>& GetCharacters();
  void SetBoots(const std::vector<Boot>& val);
  void SetArmors(const std::vector<Armor>& val);
  void SetCharacters(const std::vector<Character>& val);
  const Boot* GetBootFromName(const std::string& key) const;
  Boot* GetBootFromName(const std::string& key);
  const Boot* GetBoot(uint32_t idx) const;
  Boot* GetBoot(uint32_t idx);
  const Armor* GetArmorFromName(const std::string& key) const;
  Armor* GetArmorFromName(const std::string& key);
  const Armor* GetArmorFromAlternativeName(const std::string& key) const;
  Armor* GetArmorFromAlternativeName(const std::string& key);
  bool AddArmor(Armor&& elem);
  const Armor* GetArmor(uint32_t idx) const;
  Armor* GetArmor(uint32_t idx);
  const Character* GetCharacterFromName(const std::string& key) const;
  Character* GetCharacterFromName(const std::string& key);
  bool AddCharacter(Character&& elem);
  const Character* GetCharacter(uint32_t idx) const;
  Character* GetCharacter(uint32_t idx);
  bool ParseFiles(const std::string& rootPath, const Converter& converterArg);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  bool AddBoot(Boot&& elem);
  std::vector<Boot> mBoots;
  std::vector<Armor> mArmors;
  std::vector<Character> mCharacters;
  std::unordered_map<std::string, uint32_t> mBootsNameIndex;
  std::map<std::string, uint32_t> mArmorsNameIndex;
  std::map<std::string, uint32_t> mArmorsAlternativeNameIndex;
  std::map<std::string, uint32_t> mCharactersNameIndex;
};
}
