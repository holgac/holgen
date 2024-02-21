#include "Race.h"

#include "JsonHelper.h"

namespace holgen_blackbox_test {
uint32_t Race::GetId() const {
  return mId;
}
void Race::SetId(uint32_t val) {
  mId = val;
}
const std::string& Race::GetName() const {
  return mName;
}
std::string& Race::GetName() {
  return mName;
}
void Race::SetName(const std::string& val) {
  mName = val;
}
const std::vector<std::string>& Race::GetHairColors() const {
  return mHairColors;
}
std::vector<std::string>& Race::GetHairColors() {
  return mHairColors;
}
void Race::SetHairColors(const std::vector<std::string>& val) {
  mHairColors = val;
}
const std::map<std::string, std::vector<std::string>>& Race::GetNames() const {
  return mNames;
}
std::map<std::string, std::vector<std::string>>& Race::GetNames() {
  return mNames;
}
void Race::SetNames(const std::map<std::string, std::vector<std::string>>& val) {
  mNames = val;
}
bool Race::ParseJson(const rapidjson::Value& json) {
  for(const auto& data: json.GetObject()) {
    const auto& name = data.name.GetString();
    if (0 == strcmp(name, "id")) {
      auto res = JsonHelper::Parse(mId, data.value);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "name")) {
      auto res = JsonHelper::Parse(mName, data.value);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "hairColors")) {
      auto res = JsonHelper::Parse(mHairColors, data.value);
      if (!res)
        return false;
    } else if (0 == strcmp(name, "names")) {
      auto res = JsonHelper::Parse(mNames, data.value);
      if (!res)
        return false;
    }
  }
  return true;
}
}
