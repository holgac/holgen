#include <gtest/gtest.h>
#include <format>
#include <sstream>
#include <lua.hpp>
#include "Weapon.h"
#include "GameData.h"
#include "GlobalPointer.h"
#include "LuaTestHelper.h"
#include "TestEnum.h"
#include "Number.h"
#include "Calculator.h"
#include "LuaHelper.h"
#include "LuaTestStructContainer.h"
#include "TestVariantStructSharedType.h"
#include "Converter.h"

#include "TestLuaCalculator.h"
#include "TestLuaFuncTable.h"
#include "TestLuaFuncTableContainer.h"
#include "TestLuaRegistryData.h"
#include <rapidjson/document.h>

using namespace holgen_blackbox_test;

namespace {}

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
  lua_pushcfunction(mState, [](lua_State *ls) -> int {
    auto calc = Calculator::ReadProxyFromLua(ls, -2);
    int32_t val;
    LuaHelper::Read(val, ls, -1);
    val += calc->GetCurVal().GetValue();
    calc->GetCurVal().SetValue(val);
    LuaHelper::Push(val, ls, false);
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
  lua_pushcfunction(mState, [](lua_State *ls) -> int {
    auto calc = Calculator::ReadProxyFromLua(ls, -2);
    auto num = Number::ReadProxyFromLua(ls, -1);
    calc->GetCurVal().SetValue(calc->GetCurVal().GetValue() - num->GetValue());
    LuaHelper::Push(calc->GetCurVal(), ls, false);
    return 1;
  });
  lua_settable(mState, -3);
  lua_setglobal(mState, "Ops");
  c.SetSubtractLuaFunc("calc_subtract");

  Number num2;
  num2.SetValue(10);
  LuaTestHelper::ExpectStack(mState, {});
  auto res = c.Subtract(mState, num2);
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
  auto res = LuaTestHelper::ElemToString(mState, -1);
  if (res != "{test1:42,test2:35}" && res != "{test2:35,test1:42}") {
    FAIL();
  }
  EXPECT_EQ(lua_gettop(mState), 1);
  lua_pop(mState, 1);
  luaL_dostring(mState, "return c.testMap['test1']");
  LuaTestHelper::ExpectStack(mState, {"42"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, Enum) {
  TestEnum::PushEnumToLua(mState);
  luaL_dostring(mState, "return TestEnum.Entry5");
  LuaTestHelper::ExpectStack(mState, {"5"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return TestEnum[5]");
  LuaTestHelper::ExpectStack(mState, {"Entry5"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, Variant) {
  TestVariantStructSharedType::CreateLuaMetatable(mState);
  TestVariantStructHuman::CreateLuaMetatable(mState);
  TestVariantStructSharedType vs;
  vs.PushGlobalToLua(mState, "vs");
  luaL_dostring(mState, "return vs.being1");
  LuaTestHelper::ExpectStack(mState, {"null"});
  lua_pop(mState, 1);

  vs.SetBeingType(TestVariantStructType::Human);
  vs.GetBeing1AsTestVariantStructHuman()->SetName("LuaTest");
  luaL_dostring(mState, "return vs.being1.name");
  LuaTestHelper::ExpectStack(mState, {"LuaTest"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, FuncTable) {
  const char *script = R"R(
SetTo30AndGetDouble = {}
SetTo30AndGetDouble.SetField = function(funcTable, container) container.field = 30 end
SetTo30AndGetDouble.GetField = function(funcTable, container) return container.field * 2 end

SetTo40AndGetTriple = {}
SetTo40AndGetTriple.SetField = function(funcTable, container) container.field = 40 end
SetTo40AndGetTriple.GetField = function(funcTable, container) return container.field * 3 end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaFuncTableContainer c;
  c.GetScript1().SetTable("SetTo30AndGetDouble");
  c.GetScript2().SetTable("SetTo40AndGetTriple");
  c.GetScript1().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 30);
  EXPECT_EQ(c.GetScript1().GetField(mState, c), 60);
  EXPECT_EQ(c.GetScript2().GetField(mState, c), 90);

  c.GetScript2().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 40);
}

TEST_F(LuaTest, FuncTableLuaCall) {
  const char *script = R"R(
SetTo30AndGetDouble = {}
SetTo30AndGetDouble.SetField = function(funcTable, container) container.field = 30 end
SetTo30AndGetDouble.GetField = function(funcTable, container) return container.field * 2 end

SetTo40AndGetTriple = {}
SetTo40AndGetTriple.SetField = function(funcTable, container) container.field = 40 end
SetTo40AndGetTriple.GetField = function(funcTable, container) return container.field * 3 end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaFuncTableContainer c;
  c.GetScript1().SetTable("SetTo30AndGetDouble");
  c.GetScript2().SetTable("SetTo40AndGetTriple");
  c.GetScript1().SetField(mState, c);
  c.PushGlobalToLua(mState, "c");
  luaL_dostring(mState, "return c.field");
  LuaTestHelper::ExpectStack(mState, {"30"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return c.script1.GetField(c.script1, c)");
  LuaTestHelper::ExpectStack(mState, {"60"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return c.script2:GetField(c)");
  LuaTestHelper::ExpectStack(mState, {"90"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, StaticFuncTable) {
  const char *script = R"R(
SetTo30AndGetDouble = {}
SetTo30AndGetDouble.SetField = function(container) container.field = 30 end
SetTo30AndGetDouble.GetField = function(container) return container.field * 2 end

SetTo40AndGetTriple = {}
SetTo40AndGetTriple.SetField = function(container) container.field = 40 end
SetTo40AndGetTriple.GetField = function(container) return container.field * 3 end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaFuncTableContainer c;
  c.GetStaticScript1().SetTable("SetTo30AndGetDouble");
  c.GetStaticScript2().SetTable("SetTo40AndGetTriple");
  c.GetStaticScript1().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 30);
  EXPECT_EQ(c.GetStaticScript1().GetField(mState, c), 60);
  EXPECT_EQ(c.GetStaticScript2().GetField(mState, c), 90);

  c.GetStaticScript2().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 40);
}

TEST_F(LuaTest, FuncTableWithSourceTable) {
  const char *script = R"R(
Scripts.SetTo30AndGetDouble = {}
Scripts.SetTo30AndGetDouble.SetField = function(funcTable, container) container.field = 30 end
Scripts.SetTo30AndGetDouble.GetField = function(funcTable, container) return container.field * 2 end

Scripts.SetTo40AndGetTriple = {}
Scripts.SetTo40AndGetTriple.SetField = function(funcTable, container) container.field = 40 end
Scripts.SetTo40AndGetTriple.GetField = function(funcTable, container) return container.field * 3 end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaFuncTableContainer c;
  c.GetScriptWithSourceTable1().SetTable("SetTo30AndGetDouble");
  c.GetScriptWithSourceTable2().SetTable("SetTo40AndGetTriple");
  c.GetScriptWithSourceTable1().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 30);
  EXPECT_EQ(c.GetScriptWithSourceTable1().GetField(mState, c), 60);
  EXPECT_EQ(c.GetScriptWithSourceTable2().GetField(mState, c), 90);

  c.GetScriptWithSourceTable2().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 40);
}

TEST_F(LuaTest, FuncTableWithSourceTableJson) {
  const char *script = R"R(
Scripts = {}
Scripts.SetTo30AndGetDouble = {}
Scripts.SetTo30AndGetDouble.SetField = function(funcTable, container) container.field = 30 end
Scripts.SetTo30AndGetDouble.GetField = function(funcTable, container) return container.field * 2 end

Scripts.SetTo40AndGetTriple = {}
Scripts.SetTo40AndGetTriple.SetField = function(funcTable, container) container.field = 40 end
Scripts.SetTo40AndGetTriple.GetField = function(funcTable, container) return container.field * 3 end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaFuncTableContainer c;
  const char *json =
      R"R({"scriptWithSourceTable1":"SetTo30AndGetDouble", "scriptWithSourceTable2":"SetTo40AndGetTriple"})R";
  rapidjson::Document doc;
  doc.Parse(json);
  c.ParseJson(doc, {});
  c.GetScriptWithSourceTable1().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 30);
  EXPECT_EQ(c.GetScriptWithSourceTable1().GetField(mState, c), 60);
  EXPECT_EQ(c.GetScriptWithSourceTable2().GetField(mState, c), 90);

  c.GetScriptWithSourceTable2().SetField(mState, c);
  EXPECT_EQ(c.GetField(), 40);
}

TEST_F(LuaTest, FuncArgModifiersPrimitive) {
  const char *script = R"R(
Func = function(calc, num) calc.lastValue.value = calc.lastValue.value + num; return calc.lastValue.value end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaCalculator calc;
  calc.PushGlobalToLua(mState, "C");
  calc.SetAddPrimitiveLuaFunc("Func");
  EXPECT_EQ(calc.GetLastValue().GetValue(), 0);
  luaL_dostring(mState, "return C:AddPrimitive(10)");
  LuaTestHelper::ExpectStack(mState, {"10"});
  EXPECT_EQ(calc.GetLastValue().GetValue(), 10);
  lua_pop(mState, 1);
  EXPECT_EQ(calc.AddPrimitive(mState, 20), 30);
}

TEST_F(LuaTest, FuncArgModifiersRef) {
  const char *script = R"R(
Func = function(calc, num) calc.lastValue.value = calc.lastValue.value + num.value; return calc.lastValue.value end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaCalculator calc;
  calc.PushGlobalToLua(mState, "C");
  calc.SetAddRefLuaFunc("Func");
  TestLuaNumber num;
  num.SetValue(10);
  num.PushGlobalToLua(mState, "n");
  EXPECT_EQ(calc.GetLastValue().GetValue(), 0);
  luaL_dostring(mState, "return C:AddRef(n)");
  LuaTestHelper::ExpectStack(mState, {"10"});
  EXPECT_EQ(calc.GetLastValue().GetValue(), 10);
  lua_pop(mState, 1);
  TestLuaNumber num2;
  num2.SetValue(20);
  EXPECT_EQ(calc.AddRef(mState, num2), 30);
}

TEST_F(LuaTest, FuncArgModifiersNullable) {
  const char *script = R"R(
Func = function(calc, num) calc.lastValue.value = calc.lastValue.value + num.value; return calc.lastValue.value end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaCalculator calc;
  calc.PushGlobalToLua(mState, "C");
  calc.SetAddNullableLuaFunc("Func");
  TestLuaNumber num;
  num.SetValue(10);
  num.PushGlobalToLua(mState, "n");
  EXPECT_EQ(calc.GetLastValue().GetValue(), 0);
  luaL_dostring(mState, "return C:AddNullable(n)");
  LuaTestHelper::ExpectStack(mState, {"10"});
  EXPECT_EQ(calc.GetLastValue().GetValue(), 10);
  lua_pop(mState, 1);
  TestLuaNumber num2;
  num2.SetValue(20);
  EXPECT_EQ(calc.AddNullable(mState, &num2), 30);
}

TEST_F(LuaTest, FuncReturnModifiersNullable) {
  const char *script = R"R(
Func = function(calc, num) calc.lastValue.value = calc.lastValue.value + num; return calc.lastValue end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaCalculator calc;
  calc.PushGlobalToLua(mState, "C");
  calc.SetReturnNullableLuaFunc("Func");
  EXPECT_EQ(calc.GetLastValue().GetValue(), 0);
  luaL_dostring(mState, "return C:ReturnNullable(10)");
  LuaTestHelper::ExpectStack(mState, {"{p:lightuserdata}"});
  auto num = TestLuaNumber::ReadProxyFromLua(mState, -1);
  lua_pop(mState, 1);
  ASSERT_NE(num, nullptr);
  EXPECT_EQ(num->GetValue(), 10);
  calc.GetLastValue().SetValue(20);
  EXPECT_EQ(num->GetValue(), 20);
}

TEST_F(LuaTest, FuncReturnModifiersRef) {
  const char *script = R"R(
Func = function(calc, num) calc.lastValue.value = calc.lastValue.value + num; return calc.lastValue end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaCalculator calc;
  calc.PushGlobalToLua(mState, "C");
  calc.SetReturnRefLuaFunc("Func");
  EXPECT_EQ(calc.GetLastValue().GetValue(), 0);
  luaL_dostring(mState, "return C:ReturnRef(10)");
  LuaTestHelper::ExpectStack(mState, {"{p:lightuserdata}"});
  auto num = TestLuaNumber::ReadProxyFromLua(mState, -1);
  lua_pop(mState, 1);
  ASSERT_NE(num, nullptr);
  EXPECT_EQ(num->GetValue(), 10);
  calc.GetLastValue().SetValue(20);
  EXPECT_EQ(num->GetValue(), 20);
}

TEST_F(LuaTest, FuncReturnModifiersNew) {
  const char *script = R"R(
Func = function(calc, num) calc.lastValue.value = calc.lastValue.value + num; return {value=calc.lastValue.value} end
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaCalculator calc;
  calc.PushGlobalToLua(mState, "C");
  calc.SetReturnNewLuaFunc("Func");
  EXPECT_EQ(calc.GetLastValue().GetValue(), 0);
  luaL_dostring(mState, "return C:ReturnNew(10)");
  LuaTestHelper::ExpectStack(mState, {"{value:10}"});
  auto num = TestLuaNumber::ReadMirrorFromLua(mState, -1);
  lua_pop(mState, 1);
  EXPECT_EQ(num.GetValue(), 10);
  calc.GetLastValue().SetValue(20);
  EXPECT_EQ(num.GetValue(), 10);
}

TEST_F(LuaTest, RegistryData) {
  const char *script = R"R(
Tester = {
  Init = function(testLuaRegistryData, initData)
    testLuaRegistryData.data.value = initData.initialValue
  end,
  Get = function(testLuaRegistryData)
    return testLuaRegistryData.data.value
  end,
  Add = function(testLuaRegistryData, val)
    testLuaRegistryData.data.value = testLuaRegistryData.data.value + val
  end
}
)R";
  LuaHelper::CreateMetatables(mState);
  luaL_dostring(mState, script);
  TestLuaRegistryData tlrd;
  tlrd.InitializeLua(mState);
  tlrd.SetTable("Tester");
  tlrd.Init(mState, [](lua_State *innerState, auto &instance) {
    lua_newtable(innerState);
    lua_pushstring(innerState, "initialValue");
    lua_pushnumber(innerState, 12);
    lua_settable(innerState, -3);
  });
  EXPECT_EQ(tlrd.Get(mState), 12);
  tlrd.Add(mState, 5);
  EXPECT_EQ(tlrd.Get(mState), 17);
  tlrd.Add(mState, 5);
  EXPECT_EQ(tlrd.Get(mState), 22);

  tlrd.PushGlobalToLua(mState, "tlrd");
  luaL_dostring(mState, "tlrd:Init({initialValue=32})");
  EXPECT_EQ(tlrd.Get(mState), 32);
  tlrd.UninitializeLua(mState);
}
