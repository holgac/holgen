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
  static std::string Uncapitalize(const std::string &str);
  static std::string GetFieldNameInLua(const std::string &fieldName, bool isRef = false);
  static std::string Replace(const std::string &source, const std::string &from,
                             const std::string &to);
  static std::string StripNamespace(const std::string &str);
  inline static const std::string IdType = "IdType";
  inline static const std::string EnumConcept = "EnumConcept";
  inline static const std::string Singleton = "Singleton";
  inline static const std::string GlobalPointer = "GlobalPointer";
  inline static const std::string GlobalPointer_GetInstance = "GetInstance";
  inline static const std::string GlobalPointer_SetInstance = "SetInstance";
  inline static const std::string FilesystemHelper = "FilesystemHelper";
  inline static const std::string FilesystemHelper_ReadFile = "ReadFile";
  inline static const std::string FilesystemHelper_DumpFile = "DumpFile";
  inline static const std::string JsonHelper = "JsonHelper";
  inline static const std::string JsonHelper_Parse = "Parse";
  inline static const std::string JsonHelper_ParseFromFile = "ParseFromFile";
  inline static const std::string JsonHelper_ParseLuaObject = "ParseLuaObject";
  inline static const std::string JsonHelper_ParseLuaRegistryObject = "ParseLuaRegistryObject";
  inline static const std::string JsonHelper_Dump = "Dump";
  inline static const std::string JsonHelper_DumpLuaObject = "DumpLuaObject";
  inline static const std::string JsonHelper_DumpLuaRegistryObject = "DumpLuaRegistryObject";
  inline static const std::string JsonHelper_DumpToFile = "DumpToFile";
  inline static const std::string JsonHelper_ParseConvertElem = "ParseConvertElem";
  inline static const std::string JsonHelper_ParseConvertKey = "ParseConvertKey";
  inline static const std::string JsonHelper_ParseConvertKeyElem = "ParseConvertKeyElem";
  inline static const std::string LuaHelper = "LuaHelper";
  inline static const std::string LuaHelper_Push = "Push";
  inline static const std::string LuaHelper_Read = "Read";
  inline static const std::string LuaHelper_Setup = "Setup";
  inline static const std::string Lua_ReadProxyObject = "ReadProxyFromLua";
  inline static const std::string Lua_ReadMirrorObject = "ReadMirrorFromLua";
  inline static const std::string Lua_PushMirrorObject = "PushMirrorToLua";
  inline static const std::string Lua_PushProxyObject = "PushToLua";
  inline static const std::string LuaTable_TableField = "table";
  inline static const std::string Lua_CustomData = "luadata";
  inline static const std::string Lua_Initialize = "InitializeLua";
  inline static const std::string Lua_Uninitialize = "UninitializeLua";
  inline static const std::string Lua_EqualsOperator = "EqualsOperator";
  inline static const std::string LuaTable_TableFieldInIndexMethod = "TABLE";
  inline static const std::string LuaPublisher_ModulesField = "Modules";
  inline static const std::string LuaPublisher_RegisterSubscriber =
      "RegisterSubscriber";
  inline static const std::string LuaPublisher_UnregisterSubscriber =
      "UnregisterSubscriber";
  inline static const std::string LuaPublisher_UnregisterSubscriberByName =
      "UnregisterSubscriberByName";
  inline static const std::string LuaPublisher_ClearSubscribers = "ClearSubscribers";
  inline static const std::string ManagedObject_Getter = "Get";
  inline static const std::string Enum_UnderlyingType = "UnderlyingType";
  inline static const std::string Converter = "Converter";
  inline static const std::string Converter_BypassField = "bypassConverters";
  inline static const std::string ParseJson = "ParseJson";
  inline static const std::string DumpJson = "DumpJson";
  // userdata is a void*. Use it only for pointer types
  inline static const std::string UserData = "userdata";
  inline static const std::string Variant = "variant";
  inline static const std::string ClassName = "CLASS_NAME";
  // TODO: move Ref here
  inline static const std::string UserDefinedSectionBegin = "HOLGEN_USER_DEFINED_BEGIN:";
  inline static const std::string UserDefinedSectionEnd = "HOLGEN_USER_DEFINED_END:";

  inline static const std::string CompositeId_IsValid = "IsValid";
  inline static const std::string CompositeId_DeletedCountSuffix = "DeletedCount";
  inline static const std::string CompositeId_NextDeletedIndexSuffix = "NextDeletedIndex";
  inline static const std::string CompositeId_RawIdxSuffix = "ByRawIdx";
  inline static const std::string Container_NextIdSuffix = "NextId";

#if defined(HOLGEN_WINDOWS)
  inline static const std::string Newline = "\n";
#else
  inline static const std::string Newline = "\n";
#endif

  static constexpr const char *GenMessage =
      "This file is generated by holgen. Do not modify manually.";
  static constexpr const char *PartialGenMessage =
      "This file is partially generated by holgen. Only modify the custom sections.";
  static constexpr const char *CSharpProjectName = "CSharpBindings";
  static constexpr const char *CSharpInitializerMethodName = "HolgenInitialize";
  static constexpr const char *CSharpDelegateSuffix = "Delegate";
  static constexpr const char *DotNetHostName = "DotNetHost";
  static constexpr const char *DotNetHost_Initialize = "Initialize";
  static constexpr const char *CSharpMirroredStructStructName = "Fields";
  static constexpr const char *CSharpMirroredStructFieldName = "Data";
  static constexpr const char *CSharpProxyObjectPointerFieldName = "HolgenPtr";
  static constexpr const char *CSharpAuxiliarySizeSuffix = "HolgenSize";
  static constexpr const char *CSharpAuxiliaryReturnValueArgName = "holgenReturnValue";
  static constexpr const char *DeferredDeleterArgumentName = "holgenDeferredDeleter";
  static constexpr const char *CSharpInterfaceName = "HolgenDotNetInterface";
  static constexpr const char *CSharpInterfaceInstanceName = "holgenDotNetInstance";
  static constexpr const char *CSharpInterfaceFree = "HolgenFree";
  static constexpr const char *CSharpInterfaceFreePtr = "HolgenFreePtr";
  static constexpr const char *CSharpInterfaceFunctionSuffix = "HolgenPtr";
  static constexpr const char *CSharpInterfaceFunctionCallerSuffix = "Caller";
  static constexpr const char *CSharpHolgenObjectArg = "holgenObject";
  static constexpr const char *DeferredDeleter = "DeferredDeleter";
  static constexpr const char *DeferredDeleterPerform = "Perform";
  static constexpr const char *DeferredDeleterPerformManaged = "PerformManaged";
  static constexpr const char *DeferredDeleterPerformManagedArray = "PerformManagedArray";
};
} // namespace holgen
