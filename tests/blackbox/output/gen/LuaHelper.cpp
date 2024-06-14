// This file is generated by holgen. Do not modify manually.
#include "LuaHelper.h"

#include "Armor.h"
#include "Boot.h"
#include "Calculator.h"
#include "Character.h"
#include "Country.h"
#include "DamageMultiplier.h"
#include "GameData.h"
#include "Human.h"
#include "HumanManager.h"
#include "LuaTestStructContainer.h"
#include "Number.h"
#include "Person.h"
#include "Race.h"
#include "RaceId.h"
#include "TestBitmap.h"
#include "TestBitmapStruct.h"
#include "TestContainerInnerStructNoId.h"
#include "TestContainerInnerStructWithId.h"
#include "TestContainerMap.h"
#include "TestContainerSet.h"
#include "TestContainerVector.h"
#include "TestEnum.h"
#include "TestEnumStruct.h"
#include "TestJsonStructMapWithConverters.h"
#include "TestJsonStructWithTags.h"
#include "TestJsonTag.h"
#include "TestJsonTagManager.h"
#include "TestLuaFuncTable.h"
#include "TestLuaFuncTableContainer.h"
#include "TestLuaFuncTableWithSourceTable.h"
#include "TestStruct.h"
#include "TestStructArray.h"
#include "TestStructArrayCustomData1.h"
#include "TestStructArrayCustomData2.h"
#include "TestStructArrayType.h"
#include "TestStructNonCopyable.h"
#include "TestStructSingleElem.h"
#include "TestStructSingleElemContainer.h"
#include "TestStructSingleElemWithId.h"
#include "TestVariantStructCat.h"
#include "TestVariantStructDifferentTypes.h"
#include "TestVariantStructExplicitType.h"
#include "TestVariantStructHuman.h"
#include "TestVariantStructSharedType.h"
#include "TestVariantStructType.h"
#include "Weapon.h"

namespace holgen_blackbox_test {
void LuaHelper::Push(std::nullptr_t , lua_State *luaState) {
  lua_pushnil(luaState);
}

void LuaHelper::Push(bool data, lua_State *luaState) {
  lua_pushboolean(luaState, data);
}

void LuaHelper::Push(double data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(float data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int16_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int32_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int64_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(int8_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(const std::string &data, lua_State *luaState) {
  lua_pushstring(luaState, data.c_str());
}

void LuaHelper::Push(uint16_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(uint32_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(uint64_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

void LuaHelper::Push(uint8_t data, lua_State *luaState) {
  lua_pushnumber(luaState, data);
}

bool LuaHelper::Read(bool &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isboolean(luaState, luaIndex))
    return false;
  data = lua_toboolean(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(double &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(float &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int16_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int32_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int64_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int8_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(std::string &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isstring(luaState, luaIndex))
    return false;
  data = lua_tostring(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint16_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint32_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint64_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint8_t &data, lua_State *luaState, int32_t luaIndex) {
  if (!lua_isnumber(luaState, luaIndex))
    return false;
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

void LuaHelper::CreateMetatables(lua_State *luaState) {
  Armor::CreateLuaMetatable(luaState);
  Boot::CreateLuaMetatable(luaState);
  Calculator::CreateLuaMetatable(luaState);
  Character::CreateLuaMetatable(luaState);
  Country::CreateLuaMetatable(luaState);
  DamageMultiplier::CreateLuaMetatable(luaState);
  GameData::CreateLuaMetatable(luaState);
  Human::CreateLuaMetatable(luaState);
  HumanManager::CreateLuaMetatable(luaState);
  LuaTestStructContainer::CreateLuaMetatable(luaState);
  Number::CreateLuaMetatable(luaState);
  Person::CreateLuaMetatable(luaState);
  Race::CreateLuaMetatable(luaState);
  RaceId::CreateLuaMetatable(luaState);
  TestBitmapStruct::CreateLuaMetatable(luaState);
  TestContainerInnerStructNoId::CreateLuaMetatable(luaState);
  TestContainerInnerStructWithId::CreateLuaMetatable(luaState);
  TestContainerMap::CreateLuaMetatable(luaState);
  TestContainerSet::CreateLuaMetatable(luaState);
  TestContainerVector::CreateLuaMetatable(luaState);
  TestEnumStruct::CreateLuaMetatable(luaState);
  TestJsonStructMapWithConverters::CreateLuaMetatable(luaState);
  TestJsonStructWithTags::CreateLuaMetatable(luaState);
  TestJsonTag::CreateLuaMetatable(luaState);
  TestJsonTagManager::CreateLuaMetatable(luaState);
  TestLuaFuncTable::CreateLuaMetatable(luaState);
  TestLuaFuncTableContainer::CreateLuaMetatable(luaState);
  TestLuaFuncTableWithSourceTable::CreateLuaMetatable(luaState);
  TestStruct::CreateLuaMetatable(luaState);
  TestStructArray::CreateLuaMetatable(luaState);
  TestStructArrayCustomData1::CreateLuaMetatable(luaState);
  TestStructArrayCustomData2::CreateLuaMetatable(luaState);
  TestStructNonCopyable::CreateLuaMetatable(luaState);
  TestStructSingleElem::CreateLuaMetatable(luaState);
  TestStructSingleElemContainer::CreateLuaMetatable(luaState);
  TestStructSingleElemWithId::CreateLuaMetatable(luaState);
  TestVariantStructCat::CreateLuaMetatable(luaState);
  TestVariantStructDifferentTypes::CreateLuaMetatable(luaState);
  TestVariantStructExplicitType::CreateLuaMetatable(luaState);
  TestVariantStructHuman::CreateLuaMetatable(luaState);
  TestVariantStructSharedType::CreateLuaMetatable(luaState);
  Weapon::CreateLuaMetatable(luaState);
  TestBitmap::PushEnumToLua(luaState);
  TestEnum::PushEnumToLua(luaState);
  TestStructArrayType::PushEnumToLua(luaState);
  TestVariantStructType::PushEnumToLua(luaState);
  lua_newtable(luaState);
  lua_setglobal(luaState, "Ops");
  lua_newtable(luaState);
  lua_setglobal(luaState, "Scripts");
}
}
