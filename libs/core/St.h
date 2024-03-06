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
    static std::string GetFieldNameInCpp(const std::string& fieldName);
    static std::string GetIndexFieldName(const std::string& fieldName, const std::string& indexedFieldName);
    static std::string GetIndexGetterName(const std::string& fieldName, const std::string& indexedFieldName);
    inline static const std::string GlobalPointer = "GlobalPointer";
    inline static const std::string FilesystemHelper = "FilesystemHelper";
    inline static const std::string FilesystemHelper_ReadFile = "ReadFile";
    inline static const std::string JsonHelper = "JsonHelper";
    inline static const std::string JsonHelper_Parse = "Parse";
    inline static const std::string LuaHelper = "LuaHelper";
    inline static const std::string LuaHelper_Push = "Push";
    inline static const std::string LuaHelper_Read = "Read";
  };

}
