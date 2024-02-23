#pragma once

#include <cstdint>
#include <string>
#include <rapidjson/document.h>
#include <lua.hpp>
#include "Converter.h"

namespace holgen_blackbox_test {
class Boot {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  const std::string& GetName() const;
  std::string& GetName();
  void SetName(const std::string& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
};
}
