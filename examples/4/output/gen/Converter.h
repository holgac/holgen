// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <functional>
#include <string>

namespace ex4_schemas {
class Converter {
public:
  bool mBypassConverters = false;
  std::function<uint32_t(const std::string &)> armorNameToId;
  std::function<uint32_t(const std::string &)> weaponNameToId;
};
}
