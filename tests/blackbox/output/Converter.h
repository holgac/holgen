#pragma once

#include <functional>
#include <cstdint>
#include <string>
#include <map>

namespace holgen_blackbox_test {
class Converter {
public:
  std::function<uint32_t(const std::string&)> raceNameToId;
  std::function<uint32_t(const std::string&)> countryNameToId;
  std::function<uint32_t(const std::string&)> cityNameToId;
  std::function<std::map<uint32_t, uint32_t>(const std::map<std::string, uint32_t>&)> raceU32Map;
protected:
private:
};
}
