#pragma once

#include <string>
#include "holgen.h"

namespace holgen {

/**
 * String operations and constants go here.
 *
 * Can later make the customizable.
 */
class St {
public:
  // TODO: separate places to put predefined types, generated classes and function names
  static bool IsIntegral(const std::string_view &str);
  static std::string Capitalize(const std::string &str);
  static std::string GetFieldNameInLua(const std::string &fieldName, bool isRef = false);
  inline static const std::string GlobalPointer = "GlobalPointer";
  inline static const std::string GlobalPointer_GetInstance = "GetInstance";
  inline static const std::string GlobalPointer_SetInstance = "SetInstance";
  inline static const std::string FilesystemHelper = "FilesystemHelper";
  inline static const std::string FilesystemHelper_ReadFile = "ReadFile";
  inline static const std::string JsonHelper = "JsonHelper";
  inline static const std::string JsonHelper_Parse = "Parse";
  inline static const std::string JsonHelper_ParseConvertElem = "ParseConvertElem";
  inline static const std::string JsonHelper_ParseConvertKey = "ParseConvertKey";
  inline static const std::string JsonHelper_ParseConvertKeyElem = "ParseConvertKeyElem";
  inline static const std::string LuaHelper = "LuaHelper";
  inline static const std::string LuaHelper_Push = "Push";
  inline static const std::string LuaHelper_Read = "Read";
  inline static const std::string Lua_ReadProxyObject = "ReadProxyFromLua";
  inline static const std::string Lua_ReadMirrorObject = "ReadMirrorFromLua";
  inline static const std::string Lua_PushMirrorObject = "PushMirrorToLua";
  inline static const std::string LuaTable_TableField = "table";
  inline static const std::string Lua_CustomData = "luadata";
  inline static const std::string Lua_Initialize = "InitializeLua";
  inline static const std::string Lua_Uninitialize = "UninitializeLua";
  inline static const std::string LuaTable_TableFieldInIndexMethod = "TABLE";
  inline static const std::string ManagedObject_Getter = "Get";
  inline static const std::string Enum_UnderlyingType = "UnderlyingType";
  inline static const std::string Converter = "Converter";
  inline static const std::string ParseJson = "ParseJson";
  // userdata is a void*. Use it only for pointer types
  inline static const std::string UserData = "userdata";
  inline static const std::string Variant = "variant";
  inline static const std::string ClassName = "CLASS_NAME";
  // TODO: move Ref here
  inline static const std::string UserDefinedSectionBegin = "HOLGEN_USER_DEFINED_BEGIN:";
  inline static const std::string UserDefinedSectionEnd = "HOLGEN_USER_DEFINED_END:";
#if defined(HOLGEN_WINDOWS)
  inline static const std::string Newline = "\n";
#else
  inline static const std::string Newline = "\n";
#endif
};
} // namespace holgen
