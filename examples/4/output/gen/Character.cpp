// This file is generated by holgen. Do not modify manually.
#include "Character.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "DataManager.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"
#include "Weapon.h"

namespace ex4_schemas {
uint32_t Character::GetId() const {
  return mId;
}

const std::string &Character::GetName() const {
  return mName;
}

std::string &Character::GetName() {
  return mName;
}

uint32_t Character::GetPartnerId() const {
  return mPartnerId;
}

const Character *Character::GetPartner() const {
  return Character::Get(mPartnerId);
}

Character *Character::GetPartner() {
  return Character::Get(mPartnerId);
}

uint32_t Character::GetWeaponId() const {
  return mWeaponId;
}

const Weapon *Character::GetWeapon() const {
  return Weapon::Get(mWeaponId);
}

Weapon *Character::GetWeapon() {
  return Weapon::Get(mWeaponId);
}

const CharacterArmor &Character::GetArmor() const {
  return mArmor;
}

CharacterArmor &Character::GetArmor() {
  return mArmor;
}

void Character::SetId(uint32_t val) {
  mId = val;
}

void Character::SetName(const std::string &val) {
  mName = val;
}

void Character::SetPartnerId(uint32_t val) {
  mPartnerId = val;
}

void Character::SetWeaponId(uint32_t val) {
  mWeaponId = val;
}

void Character::SetArmor(const CharacterArmor &val) {
  mArmor = val;
}

Character *Character::Get(uint32_t id) {
  return GlobalPointer<DataManager>::GetInstance()->GetCharacter(id);
}

Character *Character::GetFromName(const std::string &key) {
  return GlobalPointer<DataManager>::GetInstance()->GetCharacterFromName(key);
}

bool Character::operator==(const Character &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mPartnerId != rhs.mPartnerId ||
      mWeaponId != rhs.mWeaponId ||
      mArmor != rhs.mArmor
  );
}

bool Character::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
      } else if (0 == strcmp("partner", name)) {
        auto res = JsonHelper::Parse(mPartnerId, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.partner field");
      } else if (0 == strcmp("weapon", name)) {
        if (!converter.mBypassConverters) {
          std::string temp;
          auto res = JsonHelper::Parse(temp, data.value, converter, luaState);
          HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
          mWeaponId = converter.weaponNameToId(temp);
        } else {
          auto res = JsonHelper::Parse(mWeaponId, data.value, converter, luaState);
          HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
        }
      } else if (0 == strcmp("armor", name)) {
        auto res = JsonHelper::Parse(mArmor, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Character: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mName, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mPartnerId, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.partner field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      if (!converter.mBypassConverters) {
        std::string temp;
        auto res = JsonHelper::Parse(temp, (*it), converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
        mWeaponId = converter.weaponNameToId(temp);
      } else {
        auto res = JsonHelper::Parse(mWeaponId, (*it), converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.weapon field");
      }
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Character!");
      auto res = JsonHelper::Parse(mArmor, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Character.armor field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Character!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Character.");
    return false;
  }
  return true;
}

rapidjson::Value Character::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("name", JsonHelper::Dump(mName, doc, luaState), doc.GetAllocator());
  val.AddMember("partner", JsonHelper::Dump(mPartnerId, doc, luaState), doc.GetAllocator());
  val.AddMember("weapon", JsonHelper::Dump(mWeaponId, doc, luaState), doc.GetAllocator());
  val.AddMember("armor", JsonHelper::Dump(mArmor, doc, luaState), doc.GetAllocator());
  return val;
}

void Character::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "Character");
  lua_setmetatable(luaState, -2);
}

void Character::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "partnerId");
  LuaHelper::Push<true>(mPartnerId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "weaponId");
  LuaHelper::Push<true>(mWeaponId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armor");
  mArmor.PushMirrorToLua(luaState);
  lua_settable(luaState, -3);
}

void Character::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Character *Character::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting Character", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Character *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Character Character::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Character{};
  lua_pushvalue(luaState, idx);
  lua_pushnil(luaState);
  while (lua_next(luaState, -2)) {
    auto key = lua_tostring(luaState, -2);
    if (0 == strcmp("id", key)) {
      LuaHelper::Read(result.mId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("name", key)) {
      LuaHelper::Read(result.mName, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("partnerId", key)) {
      LuaHelper::Read(result.mPartnerId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("weaponId", key)) {
      LuaHelper::Read(result.mWeaponId, luaState, -1);
      lua_pop(luaState, 1);
    } else if (0 == strcmp("armor", key)) {
      if (lua_getmetatable(luaState, -1)) {
        lua_pop(luaState, 1);
        result.mArmor = *CharacterArmor::ReadProxyFromLua(luaState, -1);
      } else {
        result.mArmor = CharacterArmor::ReadMirrorFromLua(luaState, -1);
      }
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Character.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Character::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.name from lua failed!");
  } else if (0 == strcmp("partnerId", key)) {
    auto res = LuaHelper::Read(instance->mPartnerId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.partner from lua failed!");
  } else if (0 == strcmp("weaponId", key)) {
    auto res = LuaHelper::Read(instance->mWeaponId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.weapon from lua failed!");
  } else if (0 == strcmp("armor", key)) {
    auto res = LuaHelper::Read(instance->mArmor, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Character.armor from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
  }
  return 0;
}

int Character::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = Character::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling Character.operator== method with an invalid lua proxy object!");
  Character arg0Mirror;
  Character *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = Character::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = Character::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void Character::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Character::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Character::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, Character::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "Character");
}

int Character::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else if (0 == strcmp("partnerId", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.partner with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mPartnerId, luaState);
  } else if (0 == strcmp("partner", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push<false>(Character::Get(instance->mPartnerId), luaState);
  } else if (0 == strcmp("weaponId", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.weapon with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mWeaponId, luaState);
  } else if (0 == strcmp("weapon", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    LuaHelper::Push<false>(Weapon::Get(instance->mWeaponId), luaState);
  } else if (0 == strcmp("armor", key)) {
    auto instance = Character::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Character.armor with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mArmor, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Character.{}", key);
    return 0;
  }
  return 1;
}
}
