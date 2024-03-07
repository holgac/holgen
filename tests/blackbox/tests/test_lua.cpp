#include <gtest/gtest.h>
#include <format>
#include <sstream>
#include <lua.hpp>
#include "Weapon.h"
#include "GameData.h"
#include "GlobalPointer.h"

using namespace holgen_blackbox_test;

namespace {
  class LuaState {
  public:
    lua_State *mState = nullptr;

    LuaState() {
    }

    ~LuaState() {
    }

    void Init() {
      mState = luaL_newstate();
      luaL_openlibs(mState);
    }

    void Destroy() {
      lua_close(mState);
    }

    operator lua_State *() {
      return mState;
    }
  };
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

  std::string ElemToString(int idx) {
    auto luaType = lua_type(mState, idx);
    switch (luaType) {
      case LUA_TNIL:
        return "null";
      case LUA_TBOOLEAN:
        return lua_toboolean(mState, idx) ? "true" : "false";
      case LUA_TLIGHTUSERDATA:
        return "lightuserdata";
      case LUA_TNUMBER:
        return std::format("{}", lua_tonumber(mState, idx));
      case LUA_TSTRING:
        return lua_tostring(mState, idx);
      case LUA_TTABLE: {
        // necessary if idx < 0 (because pushing changes negative indices)
        lua_pushvalue(mState, idx);
        lua_pushnil(mState);
        std::stringstream ss;
        ss << "{";
        bool isFirst = true;
        while (lua_next(mState, -2)) {
          if (isFirst) {
            isFirst = false;
          } else {
            ss << ",";
          }
          auto value = ElemToString(-1);
          lua_pop(mState, 1);
          auto key = ElemToString(-1);
          ss << key << ":" << value;
        }
        // Pop the table we reinserted
        lua_pop(mState, 1);
        ss << "}";
        return ss.str();
      }
      case LUA_TFUNCTION:
        return "function";
      case LUA_TUSERDATA:
        return "userdata";
      case LUA_TTHREAD:
        return "thread";
      default:
        return std::format("unknown: {}", luaType);
    }
  }

  void ExpectStack(const std::vector<std::string> &expected) {
    std::vector<std::string> stack;
    for (int i = 0; i < lua_gettop(mState.mState); ++i) {
      stack.push_back(ElemToString(i + 1));
    }
    ASSERT_EQ(lua_gettop(mState.mState), expected.size());
    for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(ElemToString(i + 1), expected[i]);
    }

  }

  LuaState mState;
};

TEST_F(LuaTest, TestMethods) {
  ExpectStack({});
  lua_pushnumber(mState, 10);
  std::string test = "testString";
  lua_pushstring(mState, test.c_str());
  ExpectStack({"10", "testString"});
  lua_pop(mState, 2);
  ExpectStack({});
  lua_newtable(mState);
  lua_pushstring(mState, "key");
  lua_pushstring(mState,"value");
  lua_settable(mState, -3);
  ExpectStack({"{key:value}"});
  ExpectStack({"{key:value}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return 5 + 5");
  ExpectStack({"10"});
  lua_pop(mState, 1);
}

TEST_F(LuaTest, Getters) {
  Weapon weapon;
  weapon.SetDamageMin(10);
  weapon.SetDamageMax(20);
  weapon.CreateLuaMetatable(mState);
  ExpectStack({});
  weapon.PushToLua(mState);
  ExpectStack({"{p:lightuserdata}"});
  lua_setglobal(mState, "wp");
  ExpectStack({});
  luaL_dostring(mState, "return wp.damageMin + wp.damageMax");
  ExpectStack({"30"});
  lua_pop(mState, 1);
  weapon.SetDamageMax(100);
  luaL_dostring(mState, "return wp.damageMin + wp.damageMax");
  ExpectStack({"110"});
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
  ExpectStack({});
  EXPECT_EQ(weapon.GetDamageMax(), 100);
  luaL_dostring(mState, "wp.damageMax = wp.damageMin * 3");
  EXPECT_EQ(weapon.GetDamageMax(), 30);
  luaL_dostring(mState, "return wp.damageMax");
  ExpectStack({"30"});
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
  ExpectStack({"{i:lightuserdata}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return pm.alternativeName");
  ExpectStack({"Platy"});
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
  ExpectStack({std::format("{}", boot->GetId())});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return gorion.boot");
  ExpectStack({"{i:lightuserdata}"});
  lua_pop(mState, 1);
  luaL_dostring(mState, "return gorion.boot.name");
  ExpectStack({"Boots of Speed"});
  lua_pop(mState, 1);
  auto otherBoot = Boot::GetFromName("Leather Shoes");
  ASSERT_NE(otherBoot, nullptr);
  auto cmd = std::format("gorion.bootId = {}", otherBoot->GetId());
  luaL_dostring(mState, cmd.c_str());
  luaL_dostring(mState, "return gorion.boot.name");
  ExpectStack({otherBoot->GetName()});
  lua_pop(mState, 1);
}
