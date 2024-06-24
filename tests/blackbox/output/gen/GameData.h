// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <rapidjson/fwd.h>
#include "Armor.h"
#include "Boot.h"
#include "Character.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class GameData {
public:
  GameData() = default;
  GameData(const GameData &rhs) = default;
  GameData(GameData &&rhs) = default;
  ~GameData() = default;
  bool operator==(const GameData &rhs) const;
  const std::vector<Boot> &GetBoots() const;
  std::vector<Boot> &GetBoots();
  const std::vector<Armor> &GetArmors() const;
  std::vector<Armor> &GetArmors();
  const std::vector<Character> &GetCharacters() const;
  std::vector<Character> &GetCharacters();
  void SetBoots(const std::vector<Boot> &val);
  void SetArmors(const std::vector<Armor> &val);
  void SetCharacters(const std::vector<Character> &val);
  const Boot *GetBootFromName(const std::string &key) const;
  Boot *GetBootFromName(const std::string &key);
  Boot *AddBoot(Boot &&elem);
  Boot *AddBoot(Boot &elem);
  const Boot *GetBoot(uint32_t idx) const;
  Boot *GetBoot(uint32_t idx);
  size_t GetBootCount() const;
  const Armor *GetArmorFromName(const std::string &key) const;
  Armor *GetArmorFromName(const std::string &key);
  const Armor *GetArmorFromAlternativeName(const std::string &key) const;
  Armor *GetArmorFromAlternativeName(const std::string &key);
  Armor *AddArmor(Armor &&elem);
  Armor *AddArmor(Armor &elem);
  const Armor *GetArmor(uint32_t idx) const;
  Armor *GetArmor(uint32_t idx);
  size_t GetArmorCount() const;
  const Character *GetCharacterFromName(const std::string &key) const;
  Character *GetCharacterFromName(const std::string &key);
  Character *AddCharacter(Character &&elem);
  Character *AddCharacter(Character &elem);
  const Character *GetCharacter(uint32_t idx) const;
  Character *GetCharacter(uint32_t idx);
  size_t GetCharacterCount() const;
  bool ParseFiles(const std::string &rootPath, const Converter &converterArg);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static GameData *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static GameData ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  GameData &operator=(const GameData &rhs) = default;
  GameData &operator=(GameData &&rhs) = default;
private:
  static int IndexMetaMethod(lua_State *luaState);
  static int NewIndexMetaMethod(lua_State *luaState);
  std::vector<Boot> mBoots;
  std::vector<Armor> mArmors;
  std::vector<Character> mCharacters;
  std::unordered_map<std::string, uint32_t> mBootsNameIndex;
  std::map<std::string, uint32_t> mArmorsNameIndex;
  std::map<std::string, uint32_t> mArmorsAlternativeNameIndex;
  std::map<std::string, uint32_t> mCharactersNameIndex;
};
}
