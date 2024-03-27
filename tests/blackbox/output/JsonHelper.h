#pragma once

#include "holgen.h"
#include <cstdint>
#include <string>
#include <deque>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <rapidjson/document.h>
#include "Converter.h"

namespace holgen_blackbox_test {
class JsonHelper {
public:
  template <typename T>
  static bool Parse(T& out, const rapidjson::Value& json, const Converter& converter) {
    return out.ParseJson(json, converter);
  }
  template <typename T>
  static bool Parse(std::deque<T>& out, const rapidjson::Value& json, const Converter& converter) {
    if (!json.IsArray())
      return false;
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      if (!res)
        return false;
      out.emplace_back(std::move(elem));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::vector<T>& out, const rapidjson::Value& json, const Converter& converter) {
    if (!json.IsArray())
      return false;
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      if (!res)
        return false;
      out.emplace_back(std::move(elem));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::set<T>& out, const rapidjson::Value& json, const Converter& converter) {
    if (!json.IsArray())
      return false;
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      if (!res)
        return false;
      out.emplace(std::move(elem));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::unordered_set<T>& out, const rapidjson::Value& json, const Converter& converter) {
    if (!json.IsArray())
      return false;
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      if (!res)
        return false;
      out.emplace(std::move(elem));
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::map<K, V>& out, const rapidjson::Value& json, const Converter& converter) {
    if (!json.IsObject())
      return false;
    for (const auto& data: json.GetObject()) {
      K key;
      Parse(key, data.name, converter);
      auto [it, res] = out.try_emplace(key, V());
      if (!res)
        return false;
      Parse(it->second, data.value, converter);
      if (!res)
        return false;
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::unordered_map<K, V>& out, const rapidjson::Value& json, const Converter& converter) {
    if (!json.IsObject())
      return false;
    for (const auto& data: json.GetObject()) {
      K key;
      Parse(key, data.name, converter);
      auto [it, res] = out.try_emplace(key, V());
      if (!res)
        return false;
      Parse(it->second, data.value, converter);
      if (!res)
        return false;
    }
    return true;
  }
protected:
private:
};
template <>
bool JsonHelper::Parse(int8_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(int16_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(int32_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(int64_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(uint8_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(uint16_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(uint32_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(uint64_t& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(float& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(double& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(bool& out, const rapidjson::Value& json, const Converter& converter);
template <>
bool JsonHelper::Parse(std::string& out, const rapidjson::Value& json, const Converter& converter);
}
