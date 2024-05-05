#include <gtest/gtest.h>
#include <format>
#include <sstream>
#include <lua.hpp>
#include "Weapon.h"
#include "GameData.h"
#include "GlobalPointer.h"
#include "LuaTestHelper.h"
#include "Number.h"
#include "Calculator.h"
#include "LuaHelper.h"
#include "LuaTestStructContainer.h"

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

TEST_F(LuaTest, Func) {
  Number::CreateLuaMetatable(mState);
  Calculator::CreateLuaMetatable(mState);
  Calculator c;
  lua_pushcfunction(mState, [](lua_State* ls) -> int {
    auto calc = Calculator::ReadFromLua(ls, -2);
    int32_t val;
    LuaHelper::Read(val, ls, -1);
    val += calc->GetCurVal().GetValue();
    calc->GetCurVal().SetValue(val);
    LuaHelper::Push(val, ls);
    return 1;
  });
  lua_setglobal(mState, "calc_add");
  c.GetCurVal().SetValue(0);
  EXPECT_EQ(c.GetCurVal().GetValue(), 0);
  c.SetAddLuaFunc("calc_add");
  LuaTestHelper::ExpectStack(mState, {});
  EXPECT_EQ(c.Add(mState, 5), 5);
  EXPECT_EQ(c.GetCurVal().GetValue(), 5);
  LuaTestHelper::ExpectStack(mState, {});
  EXPECT_EQ(c.Add(mState, 7), 12);
  EXPECT_EQ(c.GetCurVal().GetValue(), 12);
  LuaTestHelper::ExpectStack(mState, {});
  lua_newtable(mState);
  lua_pushstring(mState, "calc_subtract");
  lua_pushcfunction(mState, [](lua_State* ls) -> int {
    auto calc = Calculator::ReadFromLua(ls, -2);
    auto num = Number::ReadFromLua(ls, -1);
    calc->GetCurVal().SetValue(calc->GetCurVal().GetValue() - num->GetValue());
    LuaHelper::Push(calc->GetCurVal(), ls);
    return 1;
  });
  lua_settable(mState, -3);
  lua_setglobal(mState, "Ops");
  c.SetSubtractLuaFunc("calc_subtract");

  Number num2;
  num2.SetValue(10);
  LuaTestHelper::ExpectStack(mState, {});
  auto res = c.Subtract(mState, &num2);
  LuaTestHelper::ExpectStack(mState, {});
  ASSERT_NE(res, nullptr);
  EXPECT_EQ(res->GetValue(), 2);
  EXPECT_EQ(c.GetCurVal().GetValue(), 2);
}

TEST_F(LuaTest, CppFunc) {
  Number::CreateLuaMetatable(mState);
  Calculator::CreateLuaMetatable(mState);
  Calculator c;
  c.GetCurVal().SetValue(2);
  c.PushGlobalToLua(mState, "c");
  luaL_dostring(mState, "return c:SubtractThenMultiply(15, 12)");
  LuaTestHelper::ExpectStack(mState, {"6"});
  EXPECT_EQ(c.GetCurVal().GetValue(), 6);
  lua_pop(mState, 1);
}

TEST_F(LuaTest, ContainerVector) {
  LuaTestStructContainer::CreateLuaMetatable(mState);
  LuaTestStructContainer c;
  c.PushGlobalToLua(mState, "c");
  luaL_dostring(mState, "return c.testVector");
  LuaTestHelper::ExpectStack(mState, {"{}"});
  lua_pop(mState, 1);
  c.AddTestVectorElem(42);
  c.AddTestVectorElem(35);
  luaL_dostring(mState, "return c.testVector");
  LuaTestHelper::ExpectStack(mState, {"{1:35,0:42}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return c.testVector[0]");
  LuaTestHelper::ExpectStack(mState, {"42"});
  lua_pop(mState, 1);
}


TEST_F(LuaTest, ContainerMap) {
  LuaTestStructContainer::CreateLuaMetatable(mState);
  LuaTestStructContainer c;
  c.PushGlobalToLua(mState, "c");
  luaL_dostring(mState, "return c.testMap");
  LuaTestHelper::ExpectStack(mState, {"{}"});
  lua_pop(mState, 1);
  c.GetTestMap().emplace("test1", 42);
  c.GetTestMap().emplace("test2", 35);

  luaL_dostring(mState, "return c.testMap");
  LuaTestHelper::ExpectStack(mState, {"{test1:42,test2:35}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return c.testMap['test1']");
  LuaTestHelper::ExpectStack(mState, {"42"});
  lua_pop(mState, 1);
}
