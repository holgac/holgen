#include "LuaPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {

  namespace {

    struct LuaTypeUsage {
      std::string mValidator;
      std::string mGetter;
      std::string mPusher;
      std::string mFieldExtra;
    };

    std::map<std::string, LuaTypeUsage> LuaUsage = {
        {"int8_t",      {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"int16_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"int32_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"int64_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint8_t",     {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint16_t",    {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint32_t",    {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"uint64_t",    {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"float",       {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"double",      {"lua_isnumber",  "lua_tonumber",  "lua_pushnumber",  ""}},
        {"bool",        {"lua_isboolean", "lua_toboolean", "lua_pushboolean", ""}},
        {"std::string", {"lua_isstring",  "lua_tostring",  "lua_pushstring",  ".c_str()"}},
    };

    std::string LuaTableField_Pointer = "p";
    std::string LuaTableField_Index = "i";

  }

  void LuaPlugin::EnrichClasses() {
    for (auto &cls: mProject.mClasses) {
      if (cls.mStruct)
        EnrichClass(cls, *cls.mStruct);
      else if (cls.mEnum)
        EnrichClass(cls, *cls.mEnum);
    }
  }

  void LuaPlugin::CreateIndexMetaMethod(CodeBlock &codeBlock, Class &cls) {
    auto structDefinition = cls.mStruct;
    if (structDefinition == nullptr)
      return;
    codeBlock.Line() << "lua_pushstring(luaState, \"__index\");";
    codeBlock.Line() << "lua_pushcfunction(luaState, [](lua_State* ls) {";
    codeBlock.Indent(1);
    AddLuaInstanceGetter(codeBlock, cls, -3);
    codeBlock.Line() << "const char* key = lua_tostring(ls, -2);";
    bool isFirst = true;
    for (auto &fieldDefinition: structDefinition->mFields) {
      if (fieldDefinition.GetAnnotation(Annotations::NoLua))
        continue;
      bool isRef = fieldDefinition.mType.mName == "Ref";
      std::string rawFieldName = fieldDefinition.mName;
      if (isFirst) {
        codeBlock.Line() << "if (0 == strcmp(\"" << St::GetFieldNameInLua(fieldDefinition.mName, isRef)
                         << "\", key)) {";
        isFirst = false;
      } else {
        codeBlock.Line() << "} else if (0 == strcmp(\"" << St::GetFieldNameInLua(fieldDefinition.mName, isRef)
                         << "\", key)) {";
      }
      codeBlock.Indent(1);
      codeBlock.Add("{}::{}(instance->{}, ls);", St::LuaHelper, St::LuaHelper_Push,
                    St::GetFieldNameInCpp(fieldDefinition.mName, isRef));
      codeBlock.Indent(-1);
      if (isRef) {
        codeBlock.Line() << "} else if (0 == strcmp(\"" << St::GetFieldNameInLua(fieldDefinition.mName, false)
                         << "\", key)) {";
        codeBlock.Indent(1);
        codeBlock.Add("auto ptr = {}::{}(instance->{});", fieldDefinition.mType.mTemplateParameters[0].mName,
                      St::ManagedObject_Getter, St::GetFieldNameInCpp(fieldDefinition.mName, isRef));
        codeBlock.Add("if (ptr) {{");
        codeBlock.Indent(1);
        codeBlock.Add("{}::{}(*ptr, ls);", St::LuaHelper, St::LuaHelper_Push);
        codeBlock.Indent(-1);
        codeBlock.Add("}} else {{");
        codeBlock.Indent(1);
        codeBlock.Add("{}::{}(nullptr, ls);", St::LuaHelper, St::LuaHelper_Push);
        codeBlock.Indent(-1);
        codeBlock.Add("}}");
        codeBlock.Indent(-1);
      }
    }
    codeBlock.Line() << "} else {";
    codeBlock.Indent(1);
    codeBlock.Line() << "return 0;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "}";
    codeBlock.Line() << "return 1;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "});"; // pushcfunction for __index

    codeBlock.Line() << "lua_settable(luaState, -3);";

  }

  void LuaPlugin::AddLuaInstanceGetter(CodeBlock &codeBlock, Class &cls, int idx) {
    bool isManaged = cls.mStruct != nullptr && cls.mStruct->GetAnnotation(Annotations::Managed) != nullptr;
    if (!isManaged) {
      codeBlock.Add("lua_pushstring(ls, \"{}\");", LuaTableField_Pointer);
      codeBlock.Add("lua_gettable(ls, {});", idx);
      codeBlock.Line() << "auto instance = (" << cls.mName << "*)lua_touserdata(ls, -1);";
    } else {
      codeBlock.Add("lua_pushstring(ls, \"{}\");", LuaTableField_Index);
      codeBlock.Add("lua_gettable(ls, {});", idx);
      auto idField = cls.GetField(St::GetFieldNameInCpp(cls.mStruct->GetIdField()->mName));
      std::string tempType = "uint64_t";
      if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
        tempType = "int64_t";
      }
      codeBlock.Add("{} id = reinterpret_cast<{}>(lua_touserdata(ls, -1));", idField->mType.mName, tempType);
      codeBlock.Add("auto instance = {}::{}(id);", cls.mName, St::ManagedObject_Getter);
    }
  }

  void LuaPlugin::CreateNewIndexMetaMethod(CodeBlock &codeBlock, Class &cls) {
    auto structDefinition = cls.mStruct;
    if (structDefinition == nullptr)
      return;
    codeBlock.Line() << "lua_pushstring(luaState, \"__newindex\");";
    codeBlock.Line() << "lua_pushcfunction(luaState, [](lua_State* ls) {";
    codeBlock.Indent(1);

    AddLuaInstanceGetter(codeBlock, cls, -4);
    codeBlock.Line() << "const char* key = lua_tostring(ls, -3);";
    bool isFirst = true;
    for (auto &fieldDefinition: structDefinition->mFields) {
      // TODO: This can be a bit more nuanced, maybe allow getting but not setting?
      if (fieldDefinition.GetAnnotation(Annotations::NoLua))
        continue;
      bool isRef = fieldDefinition.mType.mName == "Ref";
      if (isFirst) {
        codeBlock.Line() << "if (0 == strcmp(\"" << St::GetFieldNameInLua(fieldDefinition.mName, isRef)
                         << "\", key)) {";
        isFirst = false;
      } else {
        codeBlock.Line() << "} else if (0 == strcmp(\"" << St::GetFieldNameInLua(fieldDefinition.mName, isRef)
                         << "\", key)) {";
      }
      codeBlock.Indent(1);
      // TODO: This appends to containers, so a=[1] a=[2] results in a=[1,2].
      codeBlock.Add("{}::{}(instance->{}, ls, -2);", St::LuaHelper, St::LuaHelper_Read,
                    St::GetFieldNameInCpp(fieldDefinition.mName, fieldDefinition.mType.mName == "Ref"));
      codeBlock.Indent(-1);
    }
    codeBlock.Line() << "}";
    codeBlock.Line() << "return 0;";
    codeBlock.Indent(-1);
    codeBlock.Line() << "});"; // pushcfunction for __newindex

    codeBlock.Line() << "lua_settable(luaState, -3);";

  }

  void LuaPlugin::EnrichClass(Class &cls, const StructDefinition &structDefinition) {
    if (structDefinition.GetAnnotation(Annotations::NoLua))
      return;
    cls.mGlobalForwardDeclarations.Add("struct lua_State;");
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");

    auto &pushToLua = cls.mMethods.emplace_back();
    pushToLua.mName = "PushToLua";
    pushToLua.mReturnType.mName = "void";
    pushToLua.mConstness = Constness::Const;
    pushToLua.mVisibility = Visibility::Public;
    {
      auto &arg = pushToLua.mArguments.emplace_back();
      arg.mType.mName = "lua_State";
      arg.mName = "luaState";
      arg.mType.mType = PassByType::Pointer;
    }
    pushToLua.mBody.Line() << "lua_newtable(luaState);";
    bool isManaged = structDefinition.GetAnnotation(Annotations::Managed);
    if (!isManaged) {
      // TODO: debug generator? Or generate extra debug stuff that's gated behind a macro?
      // Specifying object type would help here
      pushToLua.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Pointer);
      pushToLua.mBody.Line() << "lua_pushlightuserdata(luaState, (void*)this);";
    } else {
      auto idField = cls.GetField(St::GetFieldNameInCpp(structDefinition.GetIdField()->mName));
      std::string tempType = "uint64_t";
      if (TypeInfo::Get().SignedIntegralTypes.contains(idField->mType.mName)) {
        tempType = "int64_t";
      }
      pushToLua.mBody.Line() << tempType << " id = " << idField->mName << ";";
      pushToLua.mBody.Add("lua_pushstring(luaState, \"{}\");", LuaTableField_Index);
      pushToLua.mBody.Line() << "lua_pushlightuserdata(luaState, reinterpret_cast<void*>(id));";
    }
    pushToLua.mBody.Line() << "lua_settable(luaState, -3);";
    // Do this last so that metamethods don't get called during object construction
    pushToLua.mBody.Line() << "lua_getglobal(luaState, \"" << cls.mName << "Meta\");";
    pushToLua.mBody.Line() << "lua_setmetatable(luaState, -2);";

    auto &createLuaMetatable = cls.mMethods.emplace_back();
    createLuaMetatable.mName = "CreateLuaMetatable";
    createLuaMetatable.mReturnType.mName = "void";
    createLuaMetatable.mStaticness = Staticness::Static;
    createLuaMetatable.mConstness = Constness::NotConst;
    createLuaMetatable.mVisibility = Visibility::Public;
    {
      auto &arg = createLuaMetatable.mArguments.emplace_back();
      arg.mType.mName = "lua_State";
      arg.mName = "luaState";
      arg.mType.mType = PassByType::Pointer;
    }
    createLuaMetatable.mBody.Line() << "lua_newtable(luaState);";
    CreateIndexMetaMethod(createLuaMetatable.mBody, cls);
    CreateNewIndexMetaMethod(createLuaMetatable.mBody, cls);

    createLuaMetatable.mBody.Line() << "lua_setglobal(luaState, \"" << cls.mName << "Meta\");";
  }

  void LuaPlugin::EnrichClass(Class &cls, const EnumDefinition &enumDefinition) {
    if (enumDefinition.GetAnnotation(Annotations::NoLua))
      return;

    cls.mGlobalForwardDeclarations.Add("struct lua_State;");
    cls.mSourceIncludes.AddLibHeader("lua.hpp");
    cls.mSourceIncludes.AddLocalHeader(St::LuaHelper + ".h");

    auto &pushToLua = cls.mMethods.emplace_back();
    pushToLua.mName = "PushToLua";
    pushToLua.mReturnType.mName = "void";
    pushToLua.mConstness = Constness::Const;
    pushToLua.mVisibility = Visibility::Public;
    {
      auto &arg = pushToLua.mArguments.emplace_back();
      arg.mType.mName = "lua_State";
      arg.mName = "luaState";
      arg.mType.mType = PassByType::Pointer;
    }
    pushToLua.mBody.Add("{}::{}(mValue, luaState);", St::LuaHelper, St::LuaHelper_Push);
  }

}