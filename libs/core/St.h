#pragma once

#include <string>

namespace holgen {

  /**
   * String operations and constants go here.
   *
   * Can later make the customizable.
   */
  class St {
  public:
    static std::string Capitalize(const std::string &str);
    // These should use FieldDefinition.mName. NOT ClassField.mName which is modified (mField)
    static std::string GetAdderMethodName(const std::string& fieldName);
    static std::string GetGetterMethodName(const std::string& fieldName);
    static std::string GetSetterMethodName(const std::string& fieldName);
  };

}
