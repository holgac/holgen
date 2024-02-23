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

  std::string St::GetGetterMethodName(const std::string &fieldName) {
    return "Get" + Capitalize(fieldName);
  }

  std::string St::GetSetterMethodName(const std::string &fieldName) {
    return "Set" + Capitalize(fieldName);
  }

  std::string St::GetIndexFieldName(const std::string &fieldName, const std::string &indexedFieldName) {
    return GetFieldNameInCpp(fieldName) + Capitalize(indexedFieldName) + "Index";
  }

  std::string St::GetFieldNameInCpp(const std::string &fieldName) {
    return "m" + Capitalize(fieldName);
  }

  std::string St::GetIndexGetterName(const std::string &fieldName, const std::string &indexedFieldName) {
    return std::format("Get{}From{}", Capitalize(fieldName), Capitalize(indexedFieldName));
  }
}
