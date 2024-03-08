#pragma once

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

class LuaTestHelper {
public:

  static std::string ElemToString(lua_State* state, int idx) {
    auto luaType = lua_type(state, idx);
    switch (luaType) {
      case LUA_TNIL:
        return "null";
      case LUA_TBOOLEAN:
        return lua_toboolean(state, idx) ? "true" : "false";
      case LUA_TLIGHTUSERDATA:
        return "lightuserdata";
      case LUA_TNUMBER:
        return std::format("{}", lua_tonumber(state, idx));
      case LUA_TSTRING:
        return lua_tostring(state, idx);
      case LUA_TTABLE: {
        // necessary if idx < 0 (because pushing changes negative indices)
        lua_pushvalue(state, idx);
        lua_pushnil(state);
        std::stringstream ss;
        ss << "{";
        bool isFirst = true;
        while (lua_next(state, -2)) {
          if (isFirst) {
            isFirst = false;
          } else {
            ss << ",";
          }
          auto value = ElemToString(state, -1);
          lua_pop(state, 1);
          auto key = ElemToString(state, -1);
          ss << key << ":" << value;
        }
        // Pop the table we reinserted
        lua_pop(state, 1);
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

  static void ExpectStack(lua_State* state, const std::vector <std::string> &expected) {
    std::vector <std::string> stack;
    for (int i = 0; i < lua_gettop(state); ++i) {
      stack.push_back(ElemToString(state, i + 1));
    }
    ASSERT_EQ(lua_gettop(state), expected.size());
    for (int i = 0; i < expected.size(); ++i) {
      EXPECT_EQ(ElemToString(state, i + 1), expected[i]);
    }

  }

};