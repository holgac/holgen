#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"
#include "../Helpers.h"

class LuaFunctionPluginTest : public TranslatorPluginTest {
};

TEST_F(LuaFunctionPluginTest, VoidFunctionNoArgs) {
  auto project = Parse(R"R(
struct TestData {
  @luaFunc
  func TestFunction() -> void;
}
  )R");
  ClassPlugin(project).Run();
  LuaFunctionPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"void"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mFuncName_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, mFuncName_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mFuncName_TestFunction);
  lua_pop(luaState, 1);
  return {};
}
LuaHelper::Push(*this, luaState);
lua_call(luaState, 1, 0);
  )R");
}

TEST_F(LuaFunctionPluginTest, VoidFunctionInTableNoArgs) {
  auto project = Parse(R"R(
struct TestData {
  @luaFunc(table=Table)
  func TestFunction() -> void;
}
  )R");
  ClassPlugin(project).Run();
  LuaFunctionPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"void"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mFuncName_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, "Table");
lua_pushstring(luaState, mFuncName_TestFunction.c_str());
lua_gettable(luaState, -2);
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function Table.{}", mFuncName_TestFunction);
  lua_pop(luaState, 1);
  return {};
}
LuaHelper::Push(*this, luaState);
lua_call(luaState, 1, 0);
lua_pop(luaState, 1);
)R");
}

TEST_F(LuaFunctionPluginTest, FunctionNoArgs) {
  auto project = Parse(R"R(
struct TestData {
  @luaFunc
  func TestFunction() -> u32;
}
  )R");
  ClassPlugin(project).Run();
  LuaFunctionPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"uint32_t"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mFuncName_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, mFuncName_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mFuncName_TestFunction);
  lua_pop(luaState, 1);
  return {};
}
LuaHelper::Push(*this, luaState);
lua_call(luaState, 1, 1);
uint32_t result;
LuaHelper::Read(result, luaState, -1);
lua_pop(luaState, 1);
return result;
  )R");
}


TEST_F(LuaFunctionPluginTest, FunctionInTableNoArgs) {
  auto project = Parse(R"R(
struct TestData {
  @luaFunc(table=Table)
  func TestFunction() -> u32;
}
  )R");
  ClassPlugin(project).Run();
  LuaFunctionPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"uint32_t"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mFuncName_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, "Table");
lua_pushstring(luaState, mFuncName_TestFunction.c_str());
lua_gettable(luaState, -2);
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function Table.{}", mFuncName_TestFunction);
  lua_pop(luaState, 1);
  return {};
}
LuaHelper::Push(*this, luaState);
lua_call(luaState, 1, 1);
uint32_t result;
LuaHelper::Read(result, luaState, -1);
lua_pop(luaState, 2);
return result;
  )R");
}

TEST_F(LuaFunctionPluginTest, FunctionWithArgs) {
  auto project = Parse(R"R(
@managed(by=DataManager, field=inners)
struct InnerStruct{
@id u32 id;
}
struct DataManager {
  @container(elemName=inner)
  vector<InnerStruct> inners;
}
struct TestData {
  @luaFunc
  func TestFunction(s32 a1, string a2, InnerStruct a3) -> string;
}
  )R");
  ClassPlugin(project).Run();
  LuaFunctionPlugin(project).Run();
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"std::string"}, Visibility::Public, Constness::Const};
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("a1", Type{"int32_t"});
  method.mArguments.emplace_back("a2", Type{"std::string", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("a3", Type{"InnerStruct", PassByType::Pointer, Constness::Const});
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mFuncName_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, mFuncName_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mFuncName_TestFunction);
  lua_pop(luaState, 1);
  return {};
}
LuaHelper::Push(*this, luaState);
LuaHelper::Push(a1, luaState);
LuaHelper::Push(a2, luaState);
LuaHelper::Push(a3, luaState);
lua_call(luaState, 4, 1);
std::string result;
LuaHelper::Read(result, luaState, -1);
lua_pop(luaState, 1);
return result;
  )R");
}
