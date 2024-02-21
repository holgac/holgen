#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
class Race {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  const std::string& GetName() const;
  std::string& GetName();
  void SetName(const std::string& val);
  const std::vector<std::string>& GetHairColors() const;
  std::vector<std::string>& GetHairColors();
  void SetHairColors(const std::vector<std::string>& val);
  const std::map<std::string, std::vector<std::string>>& GetNames() const;
  std::map<std::string, std::vector<std::string>>& GetNames();
  void SetNames(const std::map<std::string, std::vector<std::string>>& val);
  bool ParseJson(const rapidjson::Value& json);
protected:
private:
  uint32_t mId;
  std::string mName;
  std::vector<std::string> mHairColors;
  std::map<std::string, std::vector<std::string>> mNames;
};
}
