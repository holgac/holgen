#pragma once

#include <string>
#include <rapidjson/document.h>

namespace holgen_blackbox_test {
class DamageMultiplier {
public:
  const std::string& GetWhen() const;
  std::string& GetWhen();
  void SetWhen(const std::string& val);
  float GetValue() const;
  void SetValue(float val);
  bool ParseJson(const rapidjson::Value& json);
protected:
private:
  std::string mWhen;
  float mValue;
};
}
