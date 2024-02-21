#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
class JsonHelper {
public:
  template <typename T>
  static bool Parse(T& out, const rapidjson::Value& json) {
    return out.ParseJson(json);
  }
  template <typename T>
  static bool Parse(std::vector<T>& out, const rapidjson::Value& json) {
    if (!json.IsArray())
      return false;
    for (const auto& data: json.GetArray()) {
      auto res = Parse(out.emplace_back(), data);
      if (!res)
        return false;
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::map<K, V>& out, const rapidjson::Value& json) {
    if (!json.IsObject())
      return false;
    for (const auto& data: json.GetObject()) {
      K key;
      Parse(key, data.name);
      auto [it, res] = out.try_emplace(key, V());
      if (!res)
        return false;
      res = Parse(it->second, data.value);
      if (!res)
        return false;
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::unordered_map<K, V>& out, const rapidjson::Value& json) {
    if (!json.IsObject())
      return false;
    for (const auto& data: json.GetObject()) {
      K key;
      Parse(key, data.name);
      auto [it, res] = out.try_emplace(key, V());
      if (!res)
        return false;
      res = Parse(it->second, data.value);
      if (!res)
        return false;
    }
    return true;
  }
protected:
private:
};
template <>
bool JsonHelper::Parse(bool& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(double& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(float& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(int16_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(int32_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(int64_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(int8_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(std::string& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(uint16_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(uint32_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(uint64_t& out, const rapidjson::Value& json);
template <>
bool JsonHelper::Parse(uint8_t& out, const rapidjson::Value& json);
}
