// This file is generated by holgen. Do not modify manually.
#include "JsonHelper.h"

#include "Converter.h"

namespace holgen_blackbox_csharp {
template <>
bool JsonHelper::Parse(int8_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt(), false, "Found type mismatch in json when parsing int8_t");
  out = json.GetInt();
  return true;
}

template <>
bool JsonHelper::Parse(int16_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt(), false, "Found type mismatch in json when parsing int16_t");
  out = json.GetInt();
  return true;
}

template <>
bool JsonHelper::Parse(int32_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt(), false, "Found type mismatch in json when parsing int32_t");
  out = json.GetInt();
  return true;
}

template <>
bool JsonHelper::Parse(int64_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsInt64(), false, "Found type mismatch in json when parsing int64_t");
  out = json.GetInt64();
  return true;
}

template <>
bool JsonHelper::Parse(uint8_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint(), false, "Found type mismatch in json when parsing uint8_t");
  out = json.GetUint();
  return true;
}

template <>
bool JsonHelper::Parse(uint16_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint(), false, "Found type mismatch in json when parsing uint16_t");
  out = json.GetUint();
  return true;
}

template <>
bool JsonHelper::Parse(uint32_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint(), false, "Found type mismatch in json when parsing uint32_t");
  out = json.GetUint();
  return true;
}

template <>
bool JsonHelper::Parse(uint64_t &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsUint64(), false, "Found type mismatch in json when parsing uint64_t");
  out = json.GetUint64();
  return true;
}

template <>
bool JsonHelper::Parse(float &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsNumber(), false, "Found type mismatch in json when parsing float");
  out = json.GetFloat();
  return true;
}

template <>
bool JsonHelper::Parse(double &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsNumber(), false, "Found type mismatch in json when parsing double");
  out = json.GetDouble();
  return true;
}

template <>
bool JsonHelper::Parse(bool &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsBool(), false, "Found type mismatch in json when parsing bool");
  out = json.GetBool();
  return true;
}

template <>
bool JsonHelper::Parse(std::string &out, const rapidjson::Value &json, const Converter &converter) {
  HOLGEN_WARN_AND_RETURN_IF(!json.IsString(), false, "Found type mismatch in json when parsing std::string");
  out = json.GetString();
  return true;
}
}
