// This file is generated by holgen. Do not modify manually.
#include "JsonHelper.h"

#include <cstring>
#include <fstream>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "Converter.h"

namespace holgen_blackbox_test {
template <>
bool JsonHelper::Parse(int8_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt(), false, "Found type mismatch in json when parsing int8_t");
  out = json.GetInt();
  return true;
}

template <>
bool JsonHelper::Parse(int16_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt(), false, "Found type mismatch in json when parsing int16_t");
  out = json.GetInt();
  return true;
}

template <>
bool JsonHelper::Parse(int32_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt(), false, "Found type mismatch in json when parsing int32_t");
  out = json.GetInt();
  return true;
}

template <>
bool JsonHelper::Parse(int64_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt64(), false, "Found type mismatch in json when parsing int64_t");
  out = json.GetInt64();
  return true;
}

template <>
bool JsonHelper::Parse(uint8_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint(), false, "Found type mismatch in json when parsing uint8_t");
  out = json.GetUint();
  return true;
}

template <>
bool JsonHelper::Parse(uint16_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint(), false, "Found type mismatch in json when parsing uint16_t");
  out = json.GetUint();
  return true;
}

template <>
bool JsonHelper::Parse(uint32_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint(), false, "Found type mismatch in json when parsing uint32_t");
  out = json.GetUint();
  return true;
}

template <>
bool JsonHelper::Parse(uint64_t &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint64(), false, "Found type mismatch in json when parsing uint64_t");
  out = json.GetUint64();
  return true;
}

template <>
bool JsonHelper::Parse(float &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsNumber(), false, "Found type mismatch in json when parsing float");
  out = json.GetFloat();
  return true;
}

template <>
bool JsonHelper::Parse(double &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsNumber(), false, "Found type mismatch in json when parsing double");
  out = json.GetDouble();
  return true;
}

template <>
bool JsonHelper::Parse(bool &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsBool(), false, "Found type mismatch in json when parsing bool");
  out = json.GetBool();
  return true;
}

template <>
bool JsonHelper::Parse(std::string &out, const rapidjson::Value &json, const Converter &converter, lua_State *luaState) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsString(), false, "Found type mismatch in json when parsing std::string");
  out = json.GetString();
  return true;
}

void JsonHelper::ParseLuaObject(const rapidjson::Value &json, lua_State *luaState) {
  switch (json.GetType()) {
  case rapidjson::kNullType:
    lua_pushnil(luaState);
    break;
  case rapidjson::kFalseType:
    lua_pushboolean(luaState, false);
    break;
  case rapidjson::kTrueType:
    lua_pushboolean(luaState, true);
    break;
  case rapidjson::kObjectType:
    {
      lua_newtable(luaState);
      for (const auto& data: json.GetObject()) {
        ParseLuaObject(data.name, luaState);
        ParseLuaObject(data.value, luaState);
        lua_settable(luaState, -3);
      }
    break;
    }
  case rapidjson::kArrayType:
    {
      lua_newtable(luaState);
      size_t i = 0;
      for (const auto& data: json.GetArray()) {
        ParseLuaObject(data, luaState);
        lua_rawseti(luaState, -2, i);
        ++i;
      }
    break;
    }
  case rapidjson::kStringType:
    lua_pushstring(luaState, json.GetString());
    break;
  case rapidjson::kNumberType:
    lua_pushnumber(luaState, json.GetDouble());
    break;
  }
}

int JsonHelper::ParseLuaRegistryObject(const rapidjson::Value &json, lua_State *luaState) {
  ParseLuaObject(json, luaState);
  return luaL_ref(luaState, LUA_REGISTRYINDEX);
}

void JsonHelper::DumpToFile(const std::filesystem::path &path, const rapidjson::Value &json) {
  rapidjson::StringBuffer sb;
  rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
  json.Accept(writer);
  std::ofstream out(path, std::ios_base::binary);
  out.write(sb.GetString(), sb.GetSize());
}

rapidjson::Value JsonHelper::Dump(const int8_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const int16_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const int32_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const int64_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const uint8_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const uint16_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const uint32_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const uint64_t &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const float &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const double &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const bool &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data);
}

rapidjson::Value JsonHelper::Dump(const std::string &data, rapidjson::Document &doc, lua_State *luaState) {
  return rapidjson::Value(data.c_str(), data.size(), doc.GetAllocator());
}

rapidjson::Value JsonHelper::DumpLuaObject(int idx, rapidjson::Document &doc, lua_State *luaState, bool stringify) {
  auto luaType = lua_type(luaState, idx);
  switch (luaType) {
  case LUA_TNIL:
    return rapidjson::Value(rapidjson::kNullType);
  case LUA_TBOOLEAN:
    return rapidjson::Value(lua_toboolean(luaState, idx));
  case LUA_TNUMBER:
    if (stringify) {
      auto val = std::to_string(lua_tonumber(luaState, idx));
      return rapidjson::Value(val.c_str(), val.size(), doc.GetAllocator());
    } else {
      return rapidjson::Value(lua_tonumber(luaState, idx));
    }
  case LUA_TSTRING:
    {
      auto val = lua_tostring(luaState, idx);
      return rapidjson::Value(val, std::strlen(val), doc.GetAllocator());
    }
  case LUA_TTABLE:
    {
      auto val = rapidjson::Value(rapidjson::kObjectType);
      lua_pushvalue(luaState, idx);
      lua_pushnil(luaState);
      while (lua_next(luaState, -2)) {
        auto value = DumpLuaObject(-1, doc, luaState, false);
        lua_pop(luaState, 1);
        auto key = DumpLuaObject(-1, doc, luaState, true);
        val.AddMember(key, value, doc.GetAllocator());
      }
      lua_pop(luaState, 1);
      return val;
    }
  default:
    HOLGEN_FAIL("Unexpected type when serializing lua data: {}", luaType);
  }
}

rapidjson::Value JsonHelper::DumpLuaRegistryObject(int data, rapidjson::Document &doc, lua_State *luaState) {
  lua_rawgeti(luaState, LUA_REGISTRYINDEX, data);
  auto res = DumpLuaObject(-1, doc, luaState, false);
  lua_pop(luaState, 1);
  return res;
}
}
