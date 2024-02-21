#pragma once
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
  class JsonHelper {
  public:
    template <typename T>
    static bool Parse(T& out, const rapidjson::Value& json) {
      out.ParseJson(json);
      return true;
    }
    /*
    static bool Parse(T& out, const rapidjson::Value& json) {
      out.ParseJson(json);
    }
     */
  };

  template<> bool JsonHelper::Parse(uint8_t& out, const rapidjson::Value& json);
  template<> bool JsonHelper::Parse(uint32_t& out, const rapidjson::Value& json);
  template<> bool JsonHelper::Parse(float& out, const rapidjson::Value& json);
}