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
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::deque");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::deque");
      out.push_back(std::move(elem));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::vector<T>& out, const rapidjson::Value& json, const Converter& converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::vector");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::vector");
      out.push_back(std::move(elem));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::set<T>& out, const rapidjson::Value& json, const Converter& converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::set");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::set");
      out.insert(std::move(elem));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::unordered_set<T>& out, const rapidjson::Value& json, const Converter& converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::unordered_set");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::unordered_set");
      out.insert(std::move(elem));
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::map<K, V>& out, const rapidjson::Value& json, const Converter& converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::map");
    for (const auto& data: json.GetObject()) {
      K key;
      auto res = Parse(key, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::map");
      auto [it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::map", key);
      res = Parse(it->second, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::map");
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::unordered_map<K, V>& out, const rapidjson::Value& json, const Converter& converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::unordered_map");
    for (const auto& data: json.GetObject()) {
      K key;
      auto res = Parse(key, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::unordered_map");
      auto [it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::unordered_map", key);
      res = Parse(it->second, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::unordered_map");
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
