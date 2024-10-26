#include "St.h"
#include <format>

namespace holgen {
std::string St::Capitalize(const std::string &str) {
  auto capitalizedName = str;

  if (capitalizedName[0] >= 'a' && capitalizedName[0] <= 'z') {
    capitalizedName[0] -= 'a' - 'A';
  }
  return capitalizedName;
}

std::string St::Uncapitalize(const std::string &str) {
  auto uncapitalizedName = str;

  if (uncapitalizedName[0] >= 'A' && uncapitalizedName[0] <= 'Z') {
    uncapitalizedName[0] += 'a' - 'A';
  }
  return uncapitalizedName;
}

std::string St::GetFieldNameInLua(const std::string &fieldName, bool isRef) {
  if (isRef)
    return fieldName + "Id";
  return fieldName;
}

std::string St::Replace(const std::string &source, const std::string &from, const std::string &to) {
  if (from.empty())
    return source;
  std::string out = source;
  size_t idx = 0;
  while ((idx = out.find(from, idx)) != std::string::npos) {
    out.replace(idx, from.length(), to);
    idx += to.size();
  }
  return out;
}

bool St::IsIntegral(const std::string_view &str) {
  if (str.empty() || (str.size() == 1 && str[0] == '-'))
    return false;
  size_t idx = 0;
  if (str[0] == '-') {
    idx += 1;
  }
  while (idx < str.size()) {
    auto c = str[idx];
    if (c < '0' || c > '9')
      return false;
    ++idx;
  }
  return true;
}
} // namespace holgen
