// This file is generated by holgen. Do not modify manually.
#include "Armor.h"

#include <cstring>
#include <rapidjson/document.h>
#include "Converter.h"
#include "DataManager.h"
#include "GlobalPointer.h"
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace ex4_schemas {
uint32_t Armor::GetId() const {
  return mId;
}

const std::string &Armor::GetName() const {
  return mName;
}

std::string &Armor::GetName() {
  return mName;
}

int8_t Armor::GetArmorClass() const {
  return mArmorClass;
}

void Armor::SetId(uint32_t val) {
  mId = val;
}

void Armor::SetName(const std::string &val) {
  mName = val;
}

void Armor::SetArmorClass(int8_t val) {
  mArmorClass = val;
}

Armor *Armor::Get(uint32_t id) {
  return GlobalPointer<DataManager>::GetInstance()->GetArmor(id);
}

Armor *Armor::GetFromName(const std::string &key) {
  return GlobalPointer<DataManager>::GetInstance()->GetArmorFromName(key);
}

bool Armor::operator==(const Armor &rhs) const {
  return !(
      mId != rhs.mId ||
      mName != rhs.mName ||
      mArmorClass != rhs.mArmorClass
  );
}

bool Armor::ParseJson(const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  if (json.IsObject()) {
    for (const auto &data: json.GetObject()) {
      const auto &name = data.name.GetString();
      if (0 == strcmp("name", name)) {
        auto res = JsonHelper::Parse(mName, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.name field");
      } else if (0 == strcmp("armorClass", name)) {
        auto res = JsonHelper::Parse(mArmorClass, data.value, converter, luaState);
        HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.armorClass field");
      } else {
        HOLGEN_WARN("Unexpected entry in json when parsing Armor: {}", name);
      }
    }
  } else if (json.IsArray()) {
    auto it = json.Begin();
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Armor!");
      auto res = JsonHelper::Parse(mName, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.name field");
      ++it;
    }
    {
      HOLGEN_WARN_AND_RETURN_IF(it == json.End(), false, "Exhausted elements when parsing Armor!");
      auto res = JsonHelper::Parse(mArmorClass, (*it), converter, luaState);
      HOLGEN_WARN_AND_RETURN_IF(!res, false, "Could not json-parse Armor.armorClass field");
      ++it;
    }
    HOLGEN_WARN_AND_RETURN_IF(it != json.End(), false, "Too many elements when parsing Armor!");
  } else {
    HOLGEN_WARN("Unexpected json type when parsing Armor.");
    return false;
  }
  return true;
}

rapidjson::Value Armor::DumpJson(rapidjson::Document &doc, lua_State *luaState) const {
  rapidjson::Value val(rapidjson::kObjectType);
  val.AddMember("name", JsonHelper::Dump(mName, doc, luaState), doc.GetAllocator());
  val.AddMember("armorClass", JsonHelper::Dump(mArmorClass, doc, luaState), doc.GetAllocator());
  return val;
}

void Armor::PushToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "p");
  lua_pushlightuserdata(luaState, (void *) this);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "c");
  lua_pushlightuserdata(luaState, &CLASS_NAME);
  lua_settable(luaState, -3);
  lua_getglobal(luaState, "Armor");
  lua_setmetatable(luaState, -2);
}

void Armor::PushMirrorToLua(lua_State *luaState) const {
  lua_newtable(luaState);
  lua_pushstring(luaState, "id");
  LuaHelper::Push<true>(mId, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "name");
  LuaHelper::Push<true>(mName, luaState);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "armorClass");
  LuaHelper::Push<true>(mArmorClass, luaState);
  lua_settable(luaState, -3);
}

void Armor::PushGlobalToLua(lua_State *luaState, const char *name) const {
  PushToLua(luaState);
  lua_setglobal(luaState, name);
}

Armor *Armor::ReadProxyFromLua(lua_State *luaState, int32_t idx) {
  lua_pushstring(luaState, "c");
  lua_gettable(luaState, idx - 1);
  if (!lua_isuserdata(luaState, -1)) {
    HOLGEN_WARN("Proxy object does not contain the correct metadata!");
    return nullptr;
  }
  auto className = *static_cast<const char**>(lua_touserdata(luaState, -1));
  lua_pop(luaState, 1);
  HOLGEN_WARN_AND_RETURN_IF(className != CLASS_NAME, nullptr, "Received {} instance when expecting Armor", className);
  lua_pushstring(luaState, "p");
  lua_gettable(luaState, idx - 1);
  auto ptr = (Armor *) lua_touserdata(luaState, -1);
  lua_pop(luaState, 1);
  return ptr;
}

Armor Armor::ReadMirrorFromLua(lua_State *luaState, int32_t idx) {
  auto result = Armor{};
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
    } else if (0 == strcmp("armorClass", key)) {
      LuaHelper::Read(result.mArmorClass, luaState, -1);
      lua_pop(luaState, 1);
    } else {
      HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
      lua_pop(luaState, 1);
    }
  }
  lua_pop(luaState, 1);
  return result;
}

int Armor::NewIndexMetaMethod(lua_State *luaState) {
  auto instance = Armor::ReadProxyFromLua(luaState, -3);
  const char *key = lua_tostring(luaState, -2);
  if (0 == strcmp("id", key)) {
    auto res = LuaHelper::Read(instance->mId, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Armor.id from lua failed!");
  } else if (0 == strcmp("name", key)) {
    auto res = LuaHelper::Read(instance->mName, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Armor.name from lua failed!");
  } else if (0 == strcmp("armorClass", key)) {
    auto res = LuaHelper::Read(instance->mArmorClass, luaState, -1);
    HOLGEN_WARN_IF(!res, "Assigning Armor.armorClass from lua failed!");
  } else {
    HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
  }
  return 0;
}

int Armor::EqualsOperatorCallerFromLua(lua_State *luaState) {
  auto instance = Armor::ReadProxyFromLua(luaState, -2);
  HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Calling Armor.operator== method with an invalid lua proxy object!");
  Armor arg0Mirror;
  Armor *arg0;
  if (lua_getmetatable(luaState, -1)) {
    lua_pop(luaState, 1);
    arg0 = Armor::ReadProxyFromLua(luaState, -1);
  } else {
    arg0Mirror = Armor::ReadMirrorFromLua(luaState, -1);
    arg0 = &arg0Mirror;
  }
  auto result = instance->operator==(*arg0);
  LuaHelper::Push<true>(result, luaState);
  return 1;
}

void Armor::CreateLuaMetatable(lua_State *luaState) {
  lua_newtable(luaState);
  lua_pushstring(luaState, "__index");
  lua_pushcfunction(luaState, Armor::IndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__newindex");
  lua_pushcfunction(luaState, Armor::NewIndexMetaMethod);
  lua_settable(luaState, -3);
  lua_pushstring(luaState, "__eq");
  lua_pushcfunction(luaState, Armor::EqualsOperatorCallerFromLua);
  lua_settable(luaState, -3);
  lua_setglobal(luaState, "Armor");
}

int Armor::IndexMetaMethod(lua_State *luaState) {
  const char *key = lua_tostring(luaState, -1);
  if (0 == strcmp("id", key)) {
    auto instance = Armor::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Armor.id with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mId, luaState);
  } else if (0 == strcmp("name", key)) {
    auto instance = Armor::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Armor.name with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mName, luaState);
  } else if (0 == strcmp("armorClass", key)) {
    auto instance = Armor::ReadProxyFromLua(luaState, -2);
    HOLGEN_WARN_AND_RETURN_IF(!instance, 0, "Requesting for Armor.armorClass with an invalid lua proxy object!");
    LuaHelper::Push<false>(instance->mArmorClass, luaState);
  } else {
    HOLGEN_WARN("Unexpected lua field: Armor.{}", key);
    return 0;
  }
  return 1;
}
}
