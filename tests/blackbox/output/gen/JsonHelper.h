// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <array>
#include <cstdint>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
class JsonHelper {
public:
  template <typename T>
  static bool Parse(T &out, const rapidjson::Value &json, const Converter &converter) {
    return out.ParseJson(json, converter);
  }
  template <typename SourceType, typename T, size_t C, typename ElemConverter>
  static bool ParseConvertElem(std::array<T, C> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::array");
    size_t writtenItemCount = 0;
    for (const auto& data: json.GetArray()) {
    HOLGEN_WARN_AND_RETURN_IF(writtenItemCount >= C, false, "Received more data than what the container can handle in std::array");
      SourceType elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::array");
      out[writtenItemCount] = std::move(elem);
      ++writtenItemCount;
    }
    return true;
  }
  template <typename T, size_t C>
  static bool Parse(std::array<T, C> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::array");
    size_t writtenItemCount = 0;
    for (const auto& data: json.GetArray()) {
    HOLGEN_WARN_AND_RETURN_IF(writtenItemCount >= C, false, "Received more data than what the container can handle in std::array");
      auto res = Parse(out[writtenItemCount], data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::array");
      ++writtenItemCount;
    }
    return true;
  }
  template <typename SourceType, typename T, typename ElemConverter>
  static bool ParseConvertElem(std::deque<T> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::deque");
    for (const auto& data: json.GetArray()) {
      SourceType elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::deque");
      out.push_back(std::move(elemConverter(elem)));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::deque<T> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::deque");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::deque");
      out.push_back(std::move(elem));
    }
    return true;
  }
  template <typename SourceType, typename T, typename ElemConverter>
  static bool ParseConvertElem(std::vector<T> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::vector");
    for (const auto& data: json.GetArray()) {
      SourceType elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::vector");
      out.push_back(std::move(elemConverter(elem)));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::vector<T> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::vector");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::vector");
      out.push_back(std::move(elem));
    }
    return true;
  }
  template <typename SourceType, typename T, typename ElemConverter>
  static bool ParseConvertElem(std::list<T> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::list");
    for (const auto& data: json.GetArray()) {
      SourceType elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::list");
      out.push_back(std::move(elemConverter(elem)));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::list<T> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::list");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::list");
      out.push_back(std::move(elem));
    }
    return true;
  }
  template <typename SourceType, typename T, typename ElemConverter>
  static bool ParseConvertElem(std::set<T> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::set");
    for (const auto& data: json.GetArray()) {
      SourceType elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::set");
      out.insert(std::move(elemConverter(elem)));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::set<T> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::set");
    for (const auto& data: json.GetArray()) {
      T elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::set");
      out.insert(std::move(elem));
    }
    return true;
  }
  template <typename SourceType, typename T, typename ElemConverter>
  static bool ParseConvertElem(std::unordered_set<T> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsArray(), false, "Found non-array json element when parsing std::unordered_set");
    for (const auto& data: json.GetArray()) {
      SourceType elem;
      auto res = Parse(elem, data, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing an elem of std::unordered_set");
      out.insert(std::move(elemConverter(elem)));
    }
    return true;
  }
  template <typename T>
  static bool Parse(std::unordered_set<T> &out, const rapidjson::Value &json, const Converter &converter) {
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
  static bool Parse(std::map<K, V> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::map");
    for (const auto& data: json.GetObject()) {
      K key;
      auto res = Parse(key, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::map");
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::map", key);
      res = Parse(it->second, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::map");
    }
    return true;
  }
  template <typename ElemSourceType, typename K, typename V, typename ElemConverter>
  static bool ParseConvertElem(std::map<K, V> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::map");
    for (const auto& data: json.GetObject()) {
      K key;
      auto res = Parse(key, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::map");
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::map", key);
      ElemSourceType valueRaw;
      res = Parse(valueRaw, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::map");
      it->second = std::move(elemConverter(valueRaw));
    }
    return true;
  }
  template <typename KeySourceType, typename K, typename V, typename KeyConverter>
  static bool ParseConvertKey(std::map<K, V> &out, const rapidjson::Value &json, const Converter &converter, const KeyConverter &keyConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::map");
    for (const auto& data: json.GetObject()) {
      KeySourceType keyInJson;
      auto res = Parse(keyInJson, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::map");
      K key = std::move(keyConverter(keyInJson));
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::map", key);
      res = Parse(it->second, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::map");
    }
    return true;
  }
  template <typename KeySourceType, typename ElemSourceType, typename K, typename V, typename KeyConverter, typename ElemConverter>
  static bool ParseConvertKeyElem(std::map<K, V> &out, const rapidjson::Value &json, const Converter &converter, const KeyConverter &keyConverter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::map");
    for (const auto& data: json.GetObject()) {
      KeySourceType keyInJson;
      auto res = Parse(keyInJson, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::map");
      K key = std::move(keyConverter(keyInJson));
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::map", key);
      ElemSourceType valueRaw;
      res = Parse(valueRaw, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::map");
      it->second = std::move(elemConverter(valueRaw));
    }
    return true;
  }
  template <typename K, typename V>
  static bool Parse(std::unordered_map<K, V> &out, const rapidjson::Value &json, const Converter &converter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::unordered_map");
    for (const auto& data: json.GetObject()) {
      K key;
      auto res = Parse(key, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::unordered_map");
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::unordered_map", key);
      res = Parse(it->second, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::unordered_map");
    }
    return true;
  }
  template <typename ElemSourceType, typename K, typename V, typename ElemConverter>
  static bool ParseConvertElem(std::unordered_map<K, V> &out, const rapidjson::Value &json, const Converter &converter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::unordered_map");
    for (const auto& data: json.GetObject()) {
      K key;
      auto res = Parse(key, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::unordered_map");
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::unordered_map", key);
      ElemSourceType valueRaw;
      res = Parse(valueRaw, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::unordered_map");
      it->second = std::move(elemConverter(valueRaw));
    }
    return true;
  }
  template <typename KeySourceType, typename K, typename V, typename KeyConverter>
  static bool ParseConvertKey(std::unordered_map<K, V> &out, const rapidjson::Value &json, const Converter &converter, const KeyConverter &keyConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::unordered_map");
    for (const auto& data: json.GetObject()) {
      KeySourceType keyInJson;
      auto res = Parse(keyInJson, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::unordered_map");
      K key = std::move(keyConverter(keyInJson));
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::unordered_map", key);
      res = Parse(it->second, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::unordered_map");
    }
    return true;
  }
  template <typename KeySourceType, typename ElemSourceType, typename K, typename V, typename KeyConverter, typename ElemConverter>
  static bool ParseConvertKeyElem(std::unordered_map<K, V> &out, const rapidjson::Value &json, const Converter &converter, const KeyConverter &keyConverter, const ElemConverter &elemConverter) {
    HOLGEN_WARN_AND_RETURN_IF(!json.IsObject(), false, "Found non-object json element when parsing std::unordered_map");
    for (const auto& data: json.GetObject()) {
      KeySourceType keyInJson;
      auto res = Parse(keyInJson, data.name, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing key of std::unordered_map");
      K key = std::move(keyConverter(keyInJson));
      auto[it, insertRes] = out.try_emplace(key, V());
      HOLGEN_WARN_AND_CONTINUE_IF(!insertRes, "Detected duplicate key: {} when parsing std::unordered_map", key);
      ElemSourceType valueRaw;
      res = Parse(valueRaw, data.value, converter);
      HOLGEN_WARN_AND_CONTINUE_IF(!res, "Failed parsing value of std::unordered_map");
      it->second = std::move(elemConverter(valueRaw));
    }
    return true;
  }
};
template <>
bool JsonHelper::Parse(int8_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(int16_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(int32_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(int64_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(uint8_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(uint16_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(uint32_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(uint64_t &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(float &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(double &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(bool &out, const rapidjson::Value &json, const Converter &converter);
template <>
bool JsonHelper::Parse(std::string &out, const rapidjson::Value &json, const Converter &converter);
}
