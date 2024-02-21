#include "JsonHelper.h"


namespace holgen_blackbox_test {

  template<>
  bool JsonHelper::Parse(uint32_t& out, const rapidjson::Value& json) {
    if (!json.IsUint())
      return false;
    out = json.GetUint();
    return true;
  }

  template<>
  bool JsonHelper::Parse(uint8_t& out, const rapidjson::Value& json) {
    if (!json.IsUint())
      return false;
    out = json.GetUint();
    return true;
  }

  template<>
  bool JsonHelper::Parse(float& out, const rapidjson::Value& json) {
    if (!json.IsFloat())
      return false;
    out = json.GetFloat();
    return true;
  }
}
