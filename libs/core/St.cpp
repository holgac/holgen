#include <format>
#include "St.h"

namespace holgen {
  std::string St::Capitalize(const std::string &str) {
    auto capitalizedName = str;

    if (capitalizedName[0] >= 'a' && capitalizedName[0] <= 'z') {
      capitalizedName[0] -= 'a' - 'A';
    }
    return capitalizedName;
  }

  std::string St::GetAdderMethodName(const std::string &fieldName) {
    return "Add" + Capitalize(fieldName);
  }

  std::string St::GetGetterMethodName(const std::string &fieldName, bool isRef) {
    if (isRef)
      return "Get" + Capitalize(fieldName) + "Id";
    else
      return "Get" + Capitalize(fieldName);
  }

  std::string St::GetSetterMethodName(const std::string &fieldName, bool isRef) {
    if (isRef)
      return "Set" + Capitalize(fieldName) + "Id";
    else
      return "Set" + Capitalize(fieldName);
  }

  std::string St::GetIndexFieldName(const std::string &fieldName, const std::string &indexedFieldName) {
    return GetFieldNameInCpp(fieldName) + Capitalize(indexedFieldName) + "Index";
  }

  std::string St::GetFieldNameInCpp(const std::string &fieldName, bool isRef) {
    if (isRef)
      return "m" + Capitalize(fieldName) + "Id";
    else
      return "m" + Capitalize(fieldName);
  }

  std::string St::GetIndexGetterName(const std::string &fieldName, const std::string &indexedFieldName) {
    return std::format("Get{}From{}", Capitalize(fieldName), Capitalize(indexedFieldName));
  }

  std::string St::GetFieldNameInLua(const std::string &fieldName, bool isRef) {
    if (isRef)
      return fieldName + "Id";
    return fieldName;
  }

  bool St::IsIntegral(const std::string_view& str) {
    if (str.empty() || (str.size() == 1 && str[0] == '-'))
      return false;
    size_t idx = 0;
    if (str[0] == '-') {
      idx += 1;
    }
    while(idx < str.size()) {
      auto c = str[idx];
      if (c < '0' || c > '9')
        return false;
      ++idx;
    }
    return true;
  }
}
