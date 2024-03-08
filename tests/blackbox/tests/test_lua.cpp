#include <gtest/gtest.h>
#include <format>
#include <sstream>
#include <lua.hpp>
#include "Weapon.h"
#include "GameData.h"
#include "GlobalPointer.h"
#include "LuaTestHelper.h"

using namespace holgen_blackbox_test;

namespace {
}

class LuaTest : public ::testing::Test {
protected:
  void SetUp() override {
    mState.Init();
  }

  void TearDown() override {
    // Check if all calls were checked and expected.
    EXPECT_EQ(lua_gettop(mState.mState), 0);
    mState.Destroy();
  }

  LuaState mState;
};

TEST_F(LuaTest, TestMethods) {
  LuaTestHelper::ExpectStack(mState, {});
  lua_pushnumber(mState, 10);
  std::string test = "testString";
  lua_pushstring(mState, test.c_str());
  LuaTestHelper::ExpectStack(mState, {"10", "testString"});
  lua_pop(mState, 2);
  LuaTestHelper::ExpectStack(mState, {});
  lua_newtable(mState);
  lua_pushstring(mState, "key");
  lua_pushstring(mState, "value");
  lua_settable(mState, -3);
  LuaTestHelper::ExpectStack(mState, {"{key:value}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return 5 + 5");
  LuaTestHelper::ExpectStack(mState, {"10"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, Getters) {
  Weapon weapon;
  weapon.SetDamageMin(10);
  weapon.SetDamageMax(20);
  weapon.CreateLuaMetatable(mState);
  LuaTestHelper::ExpectStack(mState, {});
  weapon.PushToLua(mState);
  LuaTestHelper::ExpectStack(mState, {"{p:lightuserdata}"});
  lua_setglobal(mState, "wp");
  LuaTestHelper::ExpectStack(mState, {});
  luaL_dostring(mState, "return wp.damageMin + wp.damageMax");
  LuaTestHelper::ExpectStack(mState, {"30"});
  lua_pop(mState, 1);
  weapon.SetDamageMax(100);
  luaL_dostring(mState, "return wp.damageMin + wp.damageMax");
  LuaTestHelper::ExpectStack(mState, {"110"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, Setters) {
  Weapon::CreateLuaMetatable(mState);
  Weapon weapon;
  weapon.SetDamageMin(10);
  weapon.SetDamageMax(20);
  weapon.PushToLua(mState);
  lua_setglobal(mState, "wp");
  luaL_dostring(mState, "wp.damageMax = 100");
  LuaTestHelper::ExpectStack(mState, {});
  EXPECT_EQ(weapon.GetDamageMax(), 100);
  luaL_dostring(mState, "wp.damageMax = wp.damageMin * 3");
  EXPECT_EQ(weapon.GetDamageMax(), 30);
  luaL_dostring(mState, "return wp.damageMax");
  LuaTestHelper::ExpectStack(mState, {"30"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, DataManager) {
  Armor::CreateLuaMetatable(mState);
  GameData::CreateLuaMetatable(mState);
  GameData gd;
  GlobalPointer<GameData>::SetInstance(&gd);
  gd.ParseFiles("gamedata", {});
  gd.GetArmorFromName("Plate Mail")->PushToLua(mState);
  lua_setglobal(mState, "pm");
  luaL_dostring(mState, "return pm");
  LuaTestHelper::ExpectStack(mState, {"{i:lightuserdata}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return pm.alternativeName");
  LuaTestHelper::ExpectStack(mState, {"Platy"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, Ref) {
  Boot::CreateLuaMetatable(mState);
  Character::CreateLuaMetatable(mState);
  GameData::CreateLuaMetatable(mState);
  GameData gd;
  GlobalPointer<GameData>::SetInstance(&gd);
  gd.ParseFiles("gamedata", {});
  auto gorion = Character::GetFromName("Gorion");
  ASSERT_NE(gorion, nullptr);
  auto boot = gorion->GetBoot();
  ASSERT_NE(boot, nullptr);
  gorion->PushToLua(mState);
  lua_setglobal(mState, "gorion");
  luaL_dostring(mState, "return gorion.bootId");
  LuaTestHelper::ExpectStack(mState, {std::format("{}", boot->GetId())});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return gorion.boot");
  LuaTestHelper::ExpectStack(mState, {"{i:lightuserdata}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return gorion.boot.name");
  LuaTestHelper::ExpectStack(mState, {"Boots of Speed"});
  lua_pop(mState, 1);
  auto otherBoot = Boot::GetFromName("Leather Shoes");
  ASSERT_NE(otherBoot, nullptr);
  auto cmd = std::format("gorion.bootId = {}", otherBoot->GetId());
  luaL_dostring(mState, cmd.c_str());
  luaL_dostring(mState, "return gorion.boot.name");
  LuaTestHelper::ExpectStack(mState, {otherBoot->GetName()});
  lua_pop(mState, 1);
}
