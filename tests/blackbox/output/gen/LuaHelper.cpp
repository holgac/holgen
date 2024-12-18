// This file is generated by holgen. Do not modify manually.
#include "LuaHelper.h"

#include "Armor.h"
#include "Boot.h"
#include "Calculator.h"
#include "Character.h"
#include "Country.h"
#include "DamageMultiplier.h"
#include "FilesystemHelper.h"
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
#include "TestCompositeIdCat.h"
#include "TestCompositeIdCompositeId.h"
#include "TestCompositeIdContainer.h"
#include "TestCompositeIdDog.h"
#include "TestCompositeIdHuman.h"
#include "TestCompositeIdObjectType.h"
#include "TestContainerEnum.h"
#include "TestContainerInnerStructNoId.h"
#include "TestContainerInnerStructWithId.h"
#include "TestContainerMap.h"
#include "TestContainerSet.h"
#include "TestContainerSkill.h"
#include "TestContainerVector.h"
#include "TestEnum.h"
#include "TestEnumDefaultValue.h"
#include "TestEnumProperty.h"
#include "TestEnumStruct.h"
#include "TestEventListener.h"
#include "TestJsonEnum.h"
#include "TestJsonStructContainer.h"
#include "TestJsonStructMapWithConverters.h"
#include "TestJsonStructWithSingleTag.h"
#include "TestJsonStructWithTags.h"
#include "TestJsonTag.h"
#include "TestJsonTagManager.h"
#include "TestLuaCalculator.h"
#include "TestLuaFuncTable.h"
#include "TestLuaFuncTableContainer.h"
#include "TestLuaFuncTableStatic.h"
#include "TestLuaFuncTableWithSourceTable.h"
#include "TestLuaNumber.h"
#include "TestLuaRegistryData.h"
#include "TestLuaStaticCppFunction.h"
#include "TestStruct.h"
#include "TestStructArray.h"
#include "TestStructArrayCustomData1.h"
#include "TestStructArrayCustomData2.h"
#include "TestStructArrayType.h"
#include "TestStructComparable.h"
#include "TestStructComparableMap.h"
#include "TestStructConstructor.h"
#include "TestStructHashable.h"
#include "TestStructHashableMap.h"
#include "TestStructNonCopyable.h"
#include "TestStructPairFields.h"
#include "TestStructSingleElem.h"
#include "TestStructSingleElemContainer.h"
#include "TestStructSingleElemWithId.h"
#include "TestStructSmartPointers.h"
#include "TestStructToString.h"
#include "TestStructVirtualMethods.h"
#include "TestVariantStructCat.h"
#include "TestVariantStructDifferentTypes.h"
#include "TestVariantStructExplicitType.h"
#include "TestVariantStructHuman.h"
#include "TestVariantStructSharedType.h"
#include "TestVariantStructType.h"
#include "Weapon.h"

namespace holgen_blackbox_test {
bool LuaHelper::Read(bool &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isboolean(luaState, luaIndex), false, "Reading from lua failed! Expected bool, got {}.", lua_typename(luaState, luaIndex));
  data = lua_toboolean(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(double &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected double, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(float &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected float, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int16_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int16_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int32_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int32_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int64_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int64_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(int8_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected int8_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(std::string &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isstring(luaState, luaIndex), false, "Reading from lua failed! Expected std::string, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tostring(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint16_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint16_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint32_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint32_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint64_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint64_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(uint8_t &data, lua_State *luaState, int32_t luaIndex) {
  HOLGEN_WARN_AND_RETURN_IF(!lua_isnumber(luaState, luaIndex), false, "Reading from lua failed! Expected uint8_t, got {}.", lua_typename(luaState, luaIndex));
  data = lua_tonumber(luaState, luaIndex);
  return true;
}

bool LuaHelper::Read(std::function<void(lua_State *)> &data, lua_State *luaState, int32_t luaIndex) {
  if (luaIndex < 0) {
    luaIndex = lua_gettop(luaState) + luaIndex + 1;
  }
  data = [luaIndex](lua_State *lsInner) {
    lua_pushvalue(lsInner, luaIndex);
  };
  return true;
}

void LuaHelper::Setup(lua_State *luaState, const std::filesystem::path &genDir) {
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
  TestCompositeIdCat::CreateLuaMetatable(luaState);
  TestCompositeIdCompositeId::CreateLuaMetatable(luaState);
  TestCompositeIdContainer::CreateLuaMetatable(luaState);
  TestCompositeIdDog::CreateLuaMetatable(luaState);
  TestCompositeIdHuman::CreateLuaMetatable(luaState);
  TestContainerEnum::CreateLuaMetatable(luaState);
  TestContainerInnerStructNoId::CreateLuaMetatable(luaState);
  TestContainerInnerStructWithId::CreateLuaMetatable(luaState);
  TestContainerMap::CreateLuaMetatable(luaState);
  TestContainerSet::CreateLuaMetatable(luaState);
  TestContainerVector::CreateLuaMetatable(luaState);
  TestEnumStruct::CreateLuaMetatable(luaState);
  TestEventListener::CreateLuaMetatable(luaState);
  TestJsonStructContainer::CreateLuaMetatable(luaState);
  TestJsonStructMapWithConverters::CreateLuaMetatable(luaState);
  TestJsonStructWithSingleTag::CreateLuaMetatable(luaState);
  TestJsonStructWithTags::CreateLuaMetatable(luaState);
  TestJsonTag::CreateLuaMetatable(luaState);
  TestJsonTagManager::CreateLuaMetatable(luaState);
  TestLuaCalculator::CreateLuaMetatable(luaState);
  TestLuaFuncTable::CreateLuaMetatable(luaState);
  TestLuaFuncTableContainer::CreateLuaMetatable(luaState);
  TestLuaFuncTableStatic::CreateLuaMetatable(luaState);
  TestLuaFuncTableWithSourceTable::CreateLuaMetatable(luaState);
  TestLuaNumber::CreateLuaMetatable(luaState);
  TestLuaRegistryData::CreateLuaMetatable(luaState);
  TestLuaStaticCppFunction::CreateLuaMetatable(luaState);
  TestStruct::CreateLuaMetatable(luaState);
  TestStructArray::CreateLuaMetatable(luaState);
  TestStructArrayCustomData1::CreateLuaMetatable(luaState);
  TestStructArrayCustomData2::CreateLuaMetatable(luaState);
  TestStructComparable::CreateLuaMetatable(luaState);
  TestStructComparableMap::CreateLuaMetatable(luaState);
  TestStructConstructor::CreateLuaMetatable(luaState);
  TestStructHashable::CreateLuaMetatable(luaState);
  TestStructHashableMap::CreateLuaMetatable(luaState);
  TestStructNonCopyable::CreateLuaMetatable(luaState);
  TestStructPairFields::CreateLuaMetatable(luaState);
  TestStructSingleElem::CreateLuaMetatable(luaState);
  TestStructSingleElemContainer::CreateLuaMetatable(luaState);
  TestStructSingleElemWithId::CreateLuaMetatable(luaState);
  TestStructSmartPointers::CreateLuaMetatable(luaState);
  TestStructToString::CreateLuaMetatable(luaState);
  TestStructVirtualMethods::CreateLuaMetatable(luaState);
  TestVariantStructCat::CreateLuaMetatable(luaState);
  TestVariantStructDifferentTypes::CreateLuaMetatable(luaState);
  TestVariantStructExplicitType::CreateLuaMetatable(luaState);
  TestVariantStructHuman::CreateLuaMetatable(luaState);
  TestVariantStructSharedType::CreateLuaMetatable(luaState);
  Weapon::CreateLuaMetatable(luaState);
  TestBitmap::PushEnumToLua(luaState);
  TestCompositeIdObjectType::PushEnumToLua(luaState);
  TestContainerSkill::PushEnumToLua(luaState);
  TestEnum::PushEnumToLua(luaState);
  TestEnumDefaultValue::PushEnumToLua(luaState);
  TestEnumProperty::PushEnumToLua(luaState);
  TestJsonEnum::PushEnumToLua(luaState);
  TestStructArrayType::PushEnumToLua(luaState);
  TestVariantStructType::PushEnumToLua(luaState);
  luaL_dofile(luaState, (genDir / "TestStaticEventPublisher.lua").string().c_str());
  lua_newtable(luaState);
  lua_setglobal(luaState, "Ops");
  lua_newtable(luaState);
  lua_setglobal(luaState, "Scripts");
}
}
