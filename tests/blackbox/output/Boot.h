#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <rapidjson/fwd.h>
#include "Converter.h"

struct lua_State;
namespace holgen_blackbox_test {
class Boot {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  const std::string& GetName() const;
  std::string& GetName();
  void SetName(const std::string& val);
  const std::string& GetColor() const;
  std::string& GetColor();
  void SetColor(const std::string& val);
  static Boot* Get(uint32_t id);
  static Boot* GetFromName(const std::string& val);
  bool ParseJson(const rapidjson::Value& json, const Converter& converter);
  void PushToLua(lua_State* luaState) const;
  static void CreateLuaMetatable(lua_State* luaState);
protected:
private:
  uint32_t mId;
  std::string mName;
  std::string mColor;
};
}
