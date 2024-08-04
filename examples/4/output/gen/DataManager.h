// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <deque>
#include <map>
#include <string>
#include <rapidjson/fwd.h>
#include "Armor.h"
#include "Character.h"
#include "Weapon.h"

struct lua_State;
namespace ex4_schemas {
  class Converter;
}
namespace ex4_schemas {
class DataManager {
public:
  DataManager() = default;
  DataManager(const DataManager &rhs) = delete;
  DataManager(DataManager &&rhs);
  ~DataManager() = default;
  bool operator==(const DataManager &rhs) const;
  const std::deque<Character> &GetCharacters() const;
  std::deque<Character> &GetCharacters();
  const std::deque<Armor> &GetArmors() const;
  std::deque<Armor> &GetArmors();
  const std::deque<Weapon> &GetWeapons() const;
  std::deque<Weapon> &GetWeapons();
  void SetCharacters(const std::deque<Character> &val);
  void SetArmors(const std::deque<Armor> &val);
  void SetWeapons(const std::deque<Weapon> &val);
  const Character *GetCharacterFromName(const std::string &key) const;
  Character *GetCharacterFromName(const std::string &key);
  Character *AddCharacter(Character &&elem);
  Character *AddCharacter(Character &elem);
  const Character *GetCharacter(uint32_t idx) const;
  Character *GetCharacter(uint32_t idx);
  void DeleteCharacter(uint32_t idx);
  size_t GetCharacterCount() const;
  const Armor *GetArmorFromName(const std::string &key) const;
  Armor *GetArmorFromName(const std::string &key);
  Armor *AddArmor(Armor &&elem);
  Armor *AddArmor(Armor &elem);
  const Armor *GetArmor(uint32_t idx) const;
  Armor *GetArmor(uint32_t idx);
  void DeleteArmor(uint32_t idx);
  size_t GetArmorCount() const;
  const Weapon *GetWeaponFromName(const std::string &key) const;
  Weapon *GetWeaponFromName(const std::string &key);
  Weapon *AddWeapon(Weapon &&elem);
  Weapon *AddWeapon(Weapon &elem);
  const Weapon *GetWeapon(uint32_t idx) const;
  Weapon *GetWeapon(uint32_t idx);
  void DeleteWeapon(uint32_t idx);
  size_t GetWeaponCount() const;
  bool ParseFiles(const std::string &rootPath, const Converter &converterArg);
  void PushToLua(lua_State *luaState) const;
  void PushMirrorToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static DataManager *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  /*
   * This only works with negative indices
   * Reads a mirror object (a table with entries that mirror the c++ data structure)
   */
  static DataManager ReadMirrorFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  DataManager &operator=(const DataManager &rhs) = delete;
  DataManager &operator=(DataManager &&rhs);
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::deque<Character> mCharacters;
  std::deque<Armor> mArmors;
  std::deque<Weapon> mWeapons;
  std::map<std::string, uint32_t> mCharactersNameIndex;
  std::map<std::string, uint32_t> mArmorsNameIndex;
  std::map<std::string, uint32_t> mWeaponsNameIndex;
};
}
