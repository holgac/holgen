#include "LuaHelperPlugin.h"
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
}

void LuaHelperPlugin::Run() {
  auto cls = Class{St::LuaHelper, mSettings.mNamespace};
  cls.mHeaderIncludes.AddLibHeader("lua.hpp");
  cls.mHeaderIncludes.AddStandardHeader("cstddef");
  GeneratePush(cls);
  GenerateRead(cls);
  GenerateCreateMetatables(cls);
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}

void LuaHelperPlugin::GeneratePush(Class &cls) {
  GenerateBasePush(cls);
  GeneratePushNil(cls);
  GeneratePushForPrimitives(cls);
  GeneratePushForContainers(cls);
}

void LuaHelperPlugin::GeneratePushForContainers(Class &cls) {
  for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
    GeneratePushForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppSets) {
    GeneratePushForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
    GeneratePushForKeyedContainer(cls, container);
  }
}

void LuaHelperPlugin::GeneratePushForKeyedContainer(Class &cls, const std::string &container) const {
  auto method = ClassMethod{
      "Push", Type{"void"},
      Visibility::Public, Constness::NotConst, Staticness::Static
  };
  method.mTemplateParameters.emplace_back("typename", "K");
  method.mTemplateParameters.emplace_back("typename", "V");

  {
    auto &data = method.mArguments.emplace_back("data", Type{container, PassByType::Reference});
    data.mType.mTemplateParameters.emplace_back("K");
    data.mType.mTemplateParameters.emplace_back("V");
  }
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");
  method.mBody.Add("for (auto& [key, value]: data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("Push(key, luaState);");
  method.mBody.Add("Push(value, luaState);");
  method.mBody.Add("lua_settable(luaState, -3);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushForPrimitives(Class &cls) {
  for (const auto &[type, usage]: LuaUsage) {
    auto method = ClassMethod{
        "Push", Type{"void"},
        Visibility::Public, Constness::NotConst, Staticness::Static};

    {
      auto &data = method.mArguments.emplace_back("data", Type{type});
      data.mType.PreventCopying();
    }
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

    method.mBody.Line() << usage.mPusher << "(luaState, data" << usage.mFieldExtra << ");";
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void LuaHelperPlugin::GenerateBasePush(Class &cls) {
  auto method = ClassMethod{
      "Push", Type{"void"},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");

  method.mArguments.emplace_back("data", Type{"T", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("if constexpr(std::is_pointer_v<T>) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if (data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("{}::{}(*data, luaState);", St::LuaHelper, St::LuaHelper_Push);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Line() << "lua_pushnil(luaState);";
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Line() << "data.PushToLua(luaState);";
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushNil(Class &cls) {
  auto method = ClassMethod{
      "Push", Type{"void"},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("", Type{"std::nullptr_t"});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Line() << "lua_pushnil(luaState);";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateRead(Class &cls) {
  GenerateBaseRead(cls);
  GenerateReadForPrimitives(cls);
  GenerateReadForContainers(cls);

}

void LuaHelperPlugin::GenerateBaseRead(Class &cls) {
  auto method = ClassMethod{
      "Read", Type{"bool"},
      Visibility::Public, Constness::NotConst, Staticness::Static
  };
  method.mTemplateParameters.emplace_back("typename", "T");

  method.mArguments.emplace_back("data", Type{"T", PassByType::Reference});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("luaIndex", Type{"int32_t"});

  // TODO: implement reading objects from lua?
  // DataManager (or container fields) should have a lua ConstructElem method that reads from
  // a lua table, calls AddElem and returns the new element. Useful for mods for programmatic insertions
  method.mBody.Line() << "// *data = T::ReadFromLua(luaState, luaIndex);";
  method.mBody.Line() << "return false; //*data != nullptr;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateReadForContainers(Class &cls) {

  for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
    GenerateReadForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppSets) {
    GenerateReadForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
    auto method = ClassMethod{
        "Read", Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    };
    method.mTemplateParameters.emplace_back("typename", "K");
    method.mTemplateParameters.emplace_back("typename", "V");

    {
      auto &data = method.mArguments.emplace_back("data", Type{container, PassByType::Reference, Constness::Const});
      data.mType.mTemplateParameters.emplace_back("K");
      data.mType.mTemplateParameters.emplace_back("V");
    }
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
    method.mBody.Add("return false;");
    // TODO: implement
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void LuaHelperPlugin::GenerateReadForPrimitives(Class &cls) {
  for (const auto &[type, usage]: LuaUsage) {
    auto method = ClassMethod{
        "Read", Type{"bool"},
        Visibility::Public, Constness::NotConst, Staticness::Static
    };
    method.mArguments.emplace_back("data", Type{type, PassByType::Reference});
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
    method.mBody.Line() << "if (!" << usage.mValidator << "(luaState, luaIndex))";
    method.mBody.Indent(1);
    method.mBody.Line() << "return false;";
    method.mBody.Indent(-1);
    method.mBody.Line() << "data = " << usage.mGetter << "(luaState, luaIndex);";
    method.mBody.Line() << "return true;";
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

// TODO: rename to InitializeLua or something
void LuaHelperPlugin::GenerateCreateMetatables(Class &cls) {
  auto method = ClassMethod{
      "CreateMetatables", Type{"void"}, Visibility::Public,
      Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  for (auto &other: mProject.mClasses) {
    // TODO: use consts for function names
    if (other.GetMethod("CreateLuaMetatable", Constness::NotConst)) {
      cls.mSourceIncludes.AddLocalHeader(other.mName + ".h");
      method.mBody.Add("{}::CreateLuaMetatable(luaState);", other.mName);
    }
    if (other.GetMethod("PushEnumToLua", Constness::NotConst)) {
      cls.mSourceIncludes.AddLocalHeader(other.mName + ".h");
      method.mBody.Add("{}::PushEnumToLua(luaState);", other.mName);
    }
  }
  std::set<std::string> processedFunctionTables;
  for (auto &other: mProject.mClasses) {
    if (!other.mStruct)
      continue;
    for (auto &func: other.mStruct->mFunctions) {
      if (!func.GetAnnotation(Annotations::LuaFunc))
        continue;
      auto table = func.GetAnnotation(Annotations::LuaFunc)->GetAttribute(Annotations::LuaFunc_Table);
      if (!table)
        continue;
      auto tableName = table->mValue.mName;
      if (processedFunctionTables.contains(tableName))
        continue;
      processedFunctionTables.insert(tableName);
      method.mBody.Add("lua_newtable(luaState);");
      method.mBody.Add("lua_setglobal(luaState, \"{}\");", tableName);
    }
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushForSingleElemContainer(Class &cls, const std::string &container) {
  auto method = ClassMethod{
      "Push", Type{"void"},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");
  bool isFixedSize = TypeInfo::Get().CppFixedSizeContainers.contains(container);
  if (isFixedSize)
    method.mTemplateParameters.emplace_back("size_t", "C");
  {
    auto &data = method.mArguments.emplace_back("data", Type{container, PassByType::Reference});
    data.mType.mTemplateParameters.emplace_back("T");
    if (isFixedSize)
      data.mType.mTemplateParameters.emplace_back("C");
  }
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");
  method.mBody.Add("int index = 0;");
  method.mBody.Add("for (auto& elem: data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("Push(elem, luaState);");
  method.mBody.Add("lua_rawseti(luaState, -2, index++);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateReadForSingleElemContainer(Class &cls, const std::string &container) {
  auto method = ClassMethod{
      "Read", Type{"bool"},
      Visibility::Public, Constness::NotConst, Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");
  bool isFixedSize = TypeInfo::Get().CppFixedSizeContainers.contains(container);
  if (isFixedSize)
    method.mTemplateParameters.emplace_back("size_t", "C");
  {
    auto &data = method.mArguments.emplace_back("data", Type{container, PassByType::Reference, Constness::Const});
    data.mType.mTemplateParameters.emplace_back("T");
    if (isFixedSize)
      data.mType.mTemplateParameters.emplace_back("C");
  }
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
  // TODO: implement with rawseti
  method.mBody.Add("return false;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
}