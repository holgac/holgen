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
    static bool IsIntegral(const std::string_view &str);
    static std::string Capitalize(const std::string &str);
    static std::string GetFieldNameInLua(const std::string& fieldName, bool isRef = false);
    inline static const std::string GlobalPointer = "GlobalPointer";
    inline static const std::string GlobalPointer_GetInstance = "GetInstance";
    inline static const std::string GlobalPointer_SetInstance = "SetInstance";
    inline static const std::string FilesystemHelper = "FilesystemHelper";
    inline static const std::string FilesystemHelper_ReadFile = "ReadFile";
    inline static const std::string JsonHelper = "JsonHelper";
    inline static const std::string JsonHelper_Parse = "Parse";
    inline static const std::string LuaHelper = "LuaHelper";
    inline static const std::string LuaHelper_Push = "Push";
    inline static const std::string LuaHelper_Read = "Read";
    // TODO: make it a function and move to Naming. Probably better to use m<funcName>_FuncHandle
    inline static const std::string LuaFuncPrefix = "mFuncName_";
    inline static const std::string ManagedObject_Getter = "Get";
    inline static const std::string Enum_UnderlyingType = "UnderlyingType";
    inline static const std::string Converter = "Converter";
    inline static const std::string ParseJson = "ParseJson";
  };

}
