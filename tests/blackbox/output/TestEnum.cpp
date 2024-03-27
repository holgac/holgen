#include "TestEnum.h"

#include <rapidjson/document.h>
#include <lua.hpp>
#include "JsonHelper.h"
#include "LuaHelper.h"

namespace holgen_blackbox_test {
const TestEnum TestEnum::Entry5(5);
const TestEnum TestEnum::Entry1(0);
const TestEnum TestEnum::Entry2(1);
TestEnum::TestEnum(TestEnum::UnderlyingType value) :
  mValue(value)
{
}
TestEnum::UnderlyingType TestEnum::Get() const {
  return mValue;
}
TestEnum TestEnum::FromString(std::string_view str) {
  if (str == "Entry5") {
    return TestEnum(5);
  } else if (str == "Entry1") {
    return TestEnum(0);
  } else if (str == "Entry2") {
    return TestEnum(1);
  } else {
    return TestEnum(TestEnum::Invalid);
  }
}
std::string TestEnum::ToString() const {
  switch (mValue) {
    case 5: return "Entry5";
    case 0: return "Entry1";
    case 1: return "Entry2";
    default: return "INVALID";
  }
}
TestEnum& TestEnum::operator =(TestEnum::UnderlyingType rhs) {
  mValue = rhs;
  return *this;
}
bool TestEnum::operator ==(TestEnum::UnderlyingType rhs) const {
  return mValue == rhs;
}
bool TestEnum::operator ==(const TestEnum& rhs) const {
  return mValue == rhs.mValue;
}
bool TestEnum::operator !=(TestEnum::UnderlyingType rhs) const {
  return mValue != rhs;
}
bool TestEnum::operator !=(const TestEnum& rhs) const {
  return mValue != rhs.mValue;
}
std::array<TestEnum, 3> TestEnum::GetEntries() {
  return std::array<TestEnum, 3>{Entry5, Entry1, Entry2};
}
bool TestEnum::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  if (json.IsString()) {
    *this = TestEnum::FromString(std::string_view(json.GetString(), json.GetStringLength()));
  } else if (json.IsInt64()) {
    *this = TestEnum(json.GetInt64());
  } else {
    *this = TestEnum(TestEnum::Invalid);
    return false;
  }
  return true;
}
void TestEnum::PushToLua(lua_State* luaState) const {
  LuaHelper::Push(mValue, luaState);
}
}
