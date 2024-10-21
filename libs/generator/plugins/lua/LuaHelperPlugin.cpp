#include "LuaHelperPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"
#include <array>

namespace holgen {
namespace {
struct LuaTypeUsage {
  std::string mValidator;
  std::string mGetter;
  std::string mPusher;
  std::string mFieldExtra;
};

std::map<std::string, LuaTypeUsage> LuaUsage = {
    {"int8_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"int16_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"int32_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"int64_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"uint8_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"uint16_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"uint32_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"uint64_t", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"float", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"double", {"lua_isnumber", "lua_tonumber", "lua_pushnumber", ""}},
    {"bool", {"lua_isboolean", "lua_toboolean", "lua_pushboolean", ""}},
    {"std::string", {"lua_isstring", "lua_tostring", "lua_pushstring", ".c_str()"}},
};
} // namespace

void LuaHelperPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::Lua)) {
    return;
  }
  auto cls = Class{St::LuaHelper, mSettings.mNamespace};
  cls.mHeaderIncludes.AddLibHeader("lua.hpp");
  cls.mHeaderIncludes.AddStandardHeader("cstddef");
  GeneratePush(cls);
  GenerateRead(cls);
  GenerateInitializeLua(cls);
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
}

void LuaHelperPlugin::GeneratePush(Class &cls) {
  GenerateBasePush(cls);
  GeneratePushNil(cls);
  GeneratePushForPrimitives(cls);
  GeneratePushForContainers(cls);
  GeneratePushTuple(cls, 2, "std::pair");
  GeneratePushForSmartPointer(cls, "std::shared_ptr");
  GeneratePushForSmartPointer(cls, "std::unique_ptr");
}

void LuaHelperPlugin::GeneratePushForContainers(Class &cls) {
  for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
    GeneratePushForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppLists) {
    GeneratePushForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppSets) {
    GeneratePushForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
    GeneratePushForKeyedContainer(cls, container);
  }
}

void LuaHelperPlugin::GeneratePushTuple(Class &cls, size_t size,
                                        const std::string &tupleClassName) {
  auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  auto &tupleArg = method.mArguments.emplace_back("data", Type{tupleClassName});
  tupleArg.mType.mType = PassByType::Reference;
  tupleArg.mType.mConstness = Constness::Const;
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");
  method.mTemplateParameters.emplace_back("bool", "PushMirror");
  for (size_t i = 0; i < size; ++i) {
    auto templateParameter = std::format("T{}", i);
    tupleArg.mType.mTemplateParameters.emplace_back(templateParameter);
    method.mTemplateParameters.emplace_back("typename", templateParameter);
    method.mBody.Add("Push<PushMirror>(std::get<{}>(data), luaState);", i);
    method.mBody.Add("lua_rawseti(luaState, -2, {});", i);
  }
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushForKeyedContainer(Class &cls,
                                                    const std::string &container) const {
  auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("bool", "PushMirror");
  method.mTemplateParameters.emplace_back("typename", "K");
  method.mTemplateParameters.emplace_back("typename", "V");

  {
    auto &data = method.mArguments.emplace_back(
        "data", Type{container, PassByType::Reference, Constness::Const});
    data.mType.mTemplateParameters.emplace_back("K");
    data.mType.mTemplateParameters.emplace_back("V");
  }
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");
  method.mBody.Add("for (auto& [key, value]: data) {{");
  method.mBody.Indent(1);
  // proxy keys don't make sense but passing PushMirror for consistency
  method.mBody.Add("Push<true>(key, luaState);");
  method.mBody.Add("Push<PushMirror>(value, luaState);");
  method.mBody.Add("lua_settable(luaState, -3);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushForPrimitives(Class &cls) {
  for (const auto &[type, usage]: LuaUsage) {
    auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                              Staticness::Static};

    method.mTemplateParameters.emplace_back("bool", "PushMirror");
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
  auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("bool", "PushMirror");
  method.mTemplateParameters.emplace_back("typename", "T");

  method.mArguments.emplace_back("data", Type{"T", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});

  method.mBody.Add("if constexpr(std::is_pointer_v<T>) {{");
  method.mBody.Indent(1);
  method.mBody.Add("if (data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("{}::{}<PushMirror>(*data, luaState);", St::LuaHelper, St::LuaHelper_Push);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Line() << "lua_pushnil(luaState);";
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add("if constexpr (PushMirror) {{");
  method.mBody.Indent(1);
  method.mBody.Add("data.{}(luaState);", St::Lua_PushMirrorObject);
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Line() << "data.PushToLua(luaState);";
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushNil(Class &cls) {
  auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("bool", "PushMirror");
  method.mArguments.emplace_back("", Type{"std::nullptr_t"});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Line() << "lua_pushnil(luaState);";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushForSmartPointer(Class &cls, const std::string &pointerType) {
  auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("bool", "PushMirror");
  method.mTemplateParameters.emplace_back("typename", "T");
  {
    auto &arg = method.mArguments.emplace_back(
        "data", Type{pointerType, PassByType::Reference, Constness::Const});
    arg.mType.mTemplateParameters.emplace_back(Type{"T"});
  }
  method.mBody.Add("static_assert(!PushMirror, \"Smart pointers cannot be pushed as mirror!\");");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("Push<PushMirror>(*data.get(), luaState);");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateRead(Class &cls) {
  GenerateBaseRead(cls);
  GenerateReadForPrimitives(cls);
  GenerateReadFunction(cls);
  GenerateReadForContainers(cls);
  GenerateReadTuple(cls, 2, "std::pair");
}

void LuaHelperPlugin::GenerateBaseRead(Class &cls) {
  auto method = ClassMethod{"Read", Type{"bool"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");

  method.mArguments.emplace_back("data", Type{"T", PassByType::Reference});
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("luaIndex", Type{"int32_t"});

  // DataManager (or container fields) should have a lua ConstructElem method
  // that reads from a lua table, calls AddElem and returns the new element.
  // Useful for mods for programmatic insertions
  method.mBody.Add("if constexpr(std::is_pointer_v<T>) {{");
  method.mBody.Indent(1);
  method.mBody.Add("return Read(*data, luaState, luaIndex);");
  method.mBody.Indent(-1);
  method.mBody.Add("}} else {{");
  method.mBody.Indent(1);
  method.mBody.Add("data = T::{}(luaState, luaIndex);", St::Lua_ReadMirrorObject);
  method.mBody.Add("return true;");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateReadForContainers(Class &cls) {
  for (const auto &container: TypeInfo::Get().CppIndexedContainers) {
    GenerateReadForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppLists) {
    GenerateReadForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppSets) {
    GenerateReadForSingleElemContainer(cls, container);
  }

  for (const auto &container: TypeInfo::Get().CppKeyedContainers) {
    auto method = ClassMethod{"Read", Type{"bool"}, Visibility::Public, Constness::NotConst,
                              Staticness::Static};
    method.mTemplateParameters.emplace_back("typename", "K");
    method.mTemplateParameters.emplace_back("typename", "V");

    {
      auto &data = method.mArguments.emplace_back(
          "data", Type{container, PassByType::Reference, Constness::NotConst});
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

void LuaHelperPlugin::GenerateReadFunction(Class &cls) {
  auto method = ClassMethod{"Read", Type{"bool"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mComments.emplace_back(
      "BEWARE: This overload assumes that the function will be called before the "
      "entry in stack is invalidated.");
  method.mComments.emplace_back("It was made specifically for forwarding lua parameters back to "
                                "lua; do not use it for anything else.");
  method.mArguments.emplace_back("data", Type{"std::function", PassByType::Reference});
  method.mArguments.back().mType.mFunctionalTemplateParameters.emplace_back("void");
  method.mArguments.back().mType.mFunctionalTemplateParameters.emplace_back("lua_State",
                                                                            PassByType::Pointer);
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
  method.mBody.Add("if (luaIndex < 0) {{");
  method.mBody.Indent(1);
  method.mBody.Add("luaIndex = lua_gettop(luaState) + luaIndex + 1;");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("data = [luaIndex](lua_State *lsInner) {{");
  method.mBody.Indent(1);
  method.mBody.Add("lua_pushvalue(lsInner, luaIndex);");
  method.mBody.Indent(-1);
  method.mBody.Add("}};");

  method.mBody.Line() << "return true;";
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateReadTuple(Class &cls, size_t size,
                                        const std::string &tupleClassName) {
  auto method = ClassMethod{"Read", Type{"bool"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  auto &tupleArg = method.mArguments.emplace_back("data", Type{tupleClassName});
  tupleArg.mType.mType = PassByType::Reference;
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
  method.mBody.Add("lua_newtable(luaState);");
  method.mBody.Add("lua_pushvalue(luaState, luaIndex);");
  method.mBody.Add("lua_pushnil(luaState);");
  for (size_t i = 0; i < size; ++i) {
    method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!lua_next(luaState, -2), false, \"Exhausted "
                     "elements when reading from lua into a {}\");",
                     tupleClassName);
    auto templateParameter = std::format("T{}", i);
    tupleArg.mType.mTemplateParameters.emplace_back(templateParameter);
    method.mTemplateParameters.emplace_back("typename", templateParameter);
    method.mBody.Add("Read(std::get<{}>(data), luaState, -1);", i);
    method.mBody.Add("lua_pop(luaState, 1);");
  }
  method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(lua_next(luaState, -2), false, \"Too many elements "
                   "when from lua into a {}\");",
                   tupleClassName);
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return true;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateReadForPrimitives(Class &cls) {
  for (const auto &[type, usage]: LuaUsage) {
    auto method = ClassMethod{"Read", Type{"bool"}, Visibility::Public, Constness::NotConst,
                              Staticness::Static};
    method.mArguments.emplace_back("data", Type{type, PassByType::Reference});
    method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
    method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
    method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!{}(luaState, luaIndex), false, \"Reading from lua "
                     "failed! Expected {}, got {{}}.\", lua_typename(luaState, luaIndex));",
                     usage.mValidator, type);
    method.mBody.Line() << "data = " << usage.mGetter << "(luaState, luaIndex);";
    method.mBody.Line() << "return true;";
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}

void LuaHelperPlugin::GenerateInitializeClasses(Class &cls, ClassMethod &method) {
  std::array<std::set<std::string>, 2> createStatements;
  for (auto &other: mProject.mClasses) {
    // TODO: use consts for function names
    if (other.GetMethod("CreateLuaMetatable", Constness::NotConst)) {
      cls.mSourceIncludes.AddLocalHeader(other.mName + ".h");
      createStatements[0].insert(std::format("{}::CreateLuaMetatable(luaState);", other.mName));
    }
    if (other.GetMethod("PushEnumToLua", Constness::NotConst)) {
      cls.mSourceIncludes.AddLocalHeader(other.mName + ".h");
      createStatements[1].insert(std::format("{}::PushEnumToLua(luaState);", other.mName));
    }
  }
  for (auto &statements: createStatements) {
    for (auto &statement: statements) {
      method.mBody.AddLine(statement);
    }
  }
}

void LuaHelperPlugin::GenerateInitializeFunctionTables(ClassMethod &method) {
  std::set<std::string> functionTablesToCreate;
  for (auto &other: mProject.mClasses) {
    if (!other.mStruct)
      continue;

    auto attrib = other.mStruct->GetMatchingAttribute(Annotations::LuaFuncTable,
                                                      Annotations::LuaFuncTable_SourceTable);
    if (attrib) {
      functionTablesToCreate.insert(attrib->mValue.mName);
    }
    for (auto &func: other.mStruct->mFunctions) {
      if (!func.GetAnnotation(Annotations::LuaFunc))
        continue;
      attrib =
          func.GetAnnotation(Annotations::LuaFunc)->GetAttribute(Annotations::LuaFunc_SourceTable);
      if (attrib) {
        functionTablesToCreate.insert(attrib->mValue.mName);
      }
    }
  }
  for (auto &table: functionTablesToCreate) {
    method.mBody.Add("lua_newtable(luaState);");
    method.mBody.Add("lua_setglobal(luaState, \"{}\");", table);
  }
}

void LuaHelperPlugin::GenerateInitializeLua(Class &cls) {
  // TODO: rename to InitializeLua or something, and put to St
  auto method = ClassMethod{"CreateMetatables", Type{"void"}, Visibility::Public,
                            Constness::NotConst, Staticness::Static};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  GenerateInitializeClasses(cls, method);
  GenerateInitializeFunctionTables(method);
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GeneratePushForSingleElemContainer(Class &cls, const std::string &container) {
  auto method = ClassMethod{"Push", Type{"void"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("bool", "PushMirror");
  method.mTemplateParameters.emplace_back("typename", "T");
  bool isFixedSize = TypeInfo::Get().CppFixedSizeContainers.contains(container);
  if (isFixedSize)
    method.mTemplateParameters.emplace_back("size_t", "C");
  {
    auto &data = method.mArguments.emplace_back(
        "data", Type{container, PassByType::Reference, Constness::Const});
    data.mType.mTemplateParameters.emplace_back("T");
    if (isFixedSize)
      data.mType.mTemplateParameters.emplace_back("C");
  }
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mBody.Add("lua_newtable(luaState);");
  method.mBody.Add("int index = 0;");
  method.mBody.Add("for (auto& elem: data) {{");
  method.mBody.Indent(1);
  method.mBody.Add("Push<PushMirror>(elem, luaState);");
  method.mBody.Add("lua_rawseti(luaState, -2, index++);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void LuaHelperPlugin::GenerateReadForSingleElemContainer(Class &cls, const std::string &container) {
  auto method = ClassMethod{"Read", Type{"bool"}, Visibility::Public, Constness::NotConst,
                            Staticness::Static};
  method.mTemplateParameters.emplace_back("typename", "T");
  bool isFixedSize = TypeInfo::Get().CppFixedSizeContainers.contains(container);
  if (isFixedSize)
    method.mTemplateParameters.emplace_back("size_t", "C");
  {
    auto &data = method.mArguments.emplace_back(
        "data", Type{container, PassByType::Reference, Constness::NotConst});
    data.mType.mTemplateParameters.emplace_back("T");
    if (isFixedSize)
      data.mType.mTemplateParameters.emplace_back("C");
  }
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("luaIndex", Type{"int32_t"});
  if (isFixedSize) {
    method.mBody.Add("size_t nextIdx = 0;");
  }
  method.mBody.Add("lua_pushvalue(luaState, luaIndex);");
  method.mBody.Add("lua_pushnil(luaState);");
  method.mBody.Add("while (lua_next(luaState, -2)) {{");
  method.mBody.Indent(1);
  if (isFixedSize) {
    method.mBody.Add("bool res = Read(data[nextIdx], luaState, -1);");
    method.mBody.Add("++nextIdx;");
  } else if (TypeInfo::Get().CppSets.contains(container)) {
    method.mBody.Add("T elem;");
    method.mBody.Add("bool res = Read(elem, luaState, -1);");
  } else {
    method.mBody.Add("bool res = Read(data.emplace_back(), luaState, -1);");
  }
  method.mBody.Add("HOLGEN_WARN_AND_RETURN_IF(!res, false, \"Could not read data from lua into a "
                   "container\");");
  if (TypeInfo::Get().CppSets.contains(container)) {
    method.mBody.Add("data.insert(elem);");
  }
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Indent(-1);
  method.mBody.Add("}}");
  method.mBody.Add("lua_pop(luaState, 1);");
  method.mBody.Add("return true;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}
} // namespace holgen
