#include "JsonHelper.h"

namespace holgen_blackbox_test {
template <>
bool JsonHelper::Parse(bool& out, const rapidjson::Value& json) {
  if (!json.IsBool())
    return false;
  out = json.GetBool();
  return true;
}
template <>
bool JsonHelper::Parse(double& out, const rapidjson::Value& json) {
  if (!json.IsDouble())
    return false;
  out = json.GetDouble();
  return true;
}
template <>
bool JsonHelper::Parse(float& out, const rapidjson::Value& json) {
  if (!json.IsFloat())
    return false;
  out = json.GetFloat();
  return true;
}
template <>
bool JsonHelper::Parse(int16_t& out, const rapidjson::Value& json) {
  if (!json.IsInt())
    return false;
  out = json.GetInt();
  return true;
}
template <>
bool JsonHelper::Parse(int32_t& out, const rapidjson::Value& json) {
  if (!json.IsInt())
    return false;
  out = json.GetInt();
  return true;
}
template <>
bool JsonHelper::Parse(int64_t& out, const rapidjson::Value& json) {
  if (!json.IsInt())
    return false;
  out = json.GetInt();
  return true;
}
template <>
bool JsonHelper::Parse(int8_t& out, const rapidjson::Value& json) {
  if (!json.IsInt())
    return false;
  out = json.GetInt();
  return true;
}
template <>
bool JsonHelper::Parse(std::string& out, const rapidjson::Value& json) {
  if (!json.IsString())
    return false;
  out = json.GetString();
  return true;
}
template <>
bool JsonHelper::Parse(uint16_t& out, const rapidjson::Value& json) {
  if (!json.IsUint())
    return false;
  out = json.GetUint();
  return true;
}
template <>
bool JsonHelper::Parse(uint32_t& out, const rapidjson::Value& json) {
  if (!json.IsUint())
    return false;
  out = json.GetUint();
  return true;
}
template <>
bool JsonHelper::Parse(uint64_t& out, const rapidjson::Value& json) {
  if (!json.IsUint())
    return false;
  out = json.GetUint();
  return true;
}
template <>
bool JsonHelper::Parse(uint8_t& out, const rapidjson::Value& json) {
  if (!json.IsUint())
    return false;
  out = json.GetUint();
  return true;
}
}
