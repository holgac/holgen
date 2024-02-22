#include "RaceId.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
uint32_t RaceId::GetId() const {
  return mId;
}
void RaceId::SetId(uint32_t val) {
  mId = val;
}
bool RaceId::ParseJson(const rapidjson::Value& json, const Converter& converter) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "id")) {
      std::string temp;
      auto res = JsonHelper::Parse(temp, data.value, converter);
      if (!res)
        return false;
      mId = converter.raceNameToId(temp);
    }
  }
  return true;
}
}
