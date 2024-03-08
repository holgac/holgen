#include "LandscapeType.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const LandscapeType LandscapeType::Land(0);
const LandscapeType LandscapeType::Sea(1);
const LandscapeType LandscapeType::River(2);
const LandscapeType LandscapeType::Mountain(3);
LandscapeType::LandscapeType(UnderlyingType value) :
  mValue(value)
{
}
LandscapeType LandscapeType::FromString(std::string_view str) {
  if (str == "Land") {
    return LandscapeType(0);
  } else if (str == "Sea") {
    return LandscapeType(1);
  } else if (str == "River") {
    return LandscapeType(2);
  } else if (str == "Mountain") {
    return LandscapeType(3);
  } else {
    return LandscapeType(LandscapeType::Invalid);
  }
}
std::string LandscapeType::ToString() const {
  switch (mValue) {
    case 0: return "Land";
    case 1: return "Sea";
    case 2: return "River";
    case 3: return "Mountain";
    default: return "INVALID";
  }
}
LandscapeType& LandscapeType::operator =(UnderlyingType rhs) {
  mValue = rhs;
  return *this;
}
bool LandscapeType::operator ==(UnderlyingType rhs) const {
  return mValue == rhs;
}
bool LandscapeType::operator !=(UnderlyingType rhs) const {
  return mValue != rhs;
}
LandscapeType& LandscapeType::operator =(const LandscapeType& rhs) {
  mValue = rhs.mValue;
  return *this;
}
bool LandscapeType::operator ==(const LandscapeType& rhs) const {
  return mValue == rhs.mValue;
}
bool LandscapeType::operator !=(const LandscapeType& rhs) const {
  return mValue != rhs.mValue;
}
bool LandscapeType::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsString()) {
    *this = LandscapeType::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = LandscapeType(json.GetInt64());
  } else {
    *this = LandscapeType(LandscapeType::Invalid);
    return false;
  }
  return true;
}
void LandscapeType::PushToLua(lua_State* luaState) const {
  LuaHelper::Push(mValue, luaState);
}
}
