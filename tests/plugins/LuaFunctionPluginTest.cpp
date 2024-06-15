#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/lua/LuaFunctionPlugin.h"

class LuaFunctionPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    LuaFunctionPlugin(project, {}).Run();
  }
};

TEST_F(LuaFunctionPluginTest, VoidFunctionNoArgs) {
  auto project = Parse(R"R(
struct TestData {
  @luaFunc
  func TestFunction();
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetField("mLuaFuncHandle_TestFunction"), nullptr);
  helpers::ExpectEqual(*cls->GetField("mLuaFuncHandle_TestFunction"),
                       ClassField{"mLuaFuncHandle_TestFunction", Type{"std::string"}});

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"void"}, Visibility::Public, Constness::Const};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_TestFunction.empty(), void(), "Calling unset TestFunction function");
lua_getglobal(luaState, mLuaFuncHandle_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mLuaFuncHandle_TestFunction);
  lua_pop(luaState, 1);
  return void();
}
LuaHelper::Push(*this, luaState);
lua_call(luaState, 1, 0);
  )R");
}

TEST_F(LuaFunctionPluginTest, VoidFunctionInTableNoArgs) {
  auto project = Parse(R"R(
struct TestData {
  @luaFunc(sourceTable=Table)
  func TestFunction() -> void;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"void"}, Visibility::Public, Constness::Const};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_TestFunction.empty(), void(), "Calling unset TestFunction function");
lua_getglobal(luaState, "Table");
lua_pushstring(luaState, mLuaFuncHandle_TestFunction.c_str());
lua_gettable(luaState, -2);
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function Table.{}", mLuaFuncHandle_TestFunction);
  lua_pop(luaState, 1);
  return void();
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
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"uint32_t"}, Visibility::Public, Constness::Const};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, mLuaFuncHandle_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mLuaFuncHandle_TestFunction);
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
  @luaFunc(sourceTable=Table)
  func TestFunction() -> u32;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"uint32_t"}, Visibility::Public, Constness::Const};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, "Table");
lua_pushstring(luaState, mLuaFuncHandle_TestFunction.c_str());
lua_gettable(luaState, -2);
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function Table.{}", mLuaFuncHandle_TestFunction);
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
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method =
      ClassMethod{"TestFunction", Type{"std::string"}, Visibility::Public, Constness::Const};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mArguments.emplace_back("a1", Type{"int32_t"});
  method.mArguments.emplace_back("a2",
                                 Type{"std::string", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("a3", Type{"InnerStruct", PassByType::Pointer, Constness::Const});
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, mLuaFuncHandle_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mLuaFuncHandle_TestFunction);
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

TEST_F(LuaFunctionPluginTest, FunctionReturningStruct) {
  // TODO: in blackbox, test this with managed, unmanaged with id and without.
  auto project = Parse(R"R(
struct InnerStruct{
@id u32 id;
}
struct TestData {
  @luaFunc
  func TestFunction() -> InnerStruct;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("TestFunction", Constness::Const), nullptr);
  auto method = ClassMethod{"TestFunction", Type{"InnerStruct", PassByType::Pointer},
                            Visibility::Public, Constness::Const};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("luaState", Type{"lua_State", PassByType::Pointer});
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::Const), method, R"R(
HOLGEN_WARN_AND_RETURN_IF(mLuaFuncHandle_TestFunction.empty(), {}, "Calling unset TestFunction function");
lua_getglobal(luaState, mLuaFuncHandle_TestFunction.c_str());
if (lua_isnil(luaState, -1)) {
  HOLGEN_WARN("Calling undefined TestFunction function {}", mLuaFuncHandle_TestFunction);
  lua_pop(luaState, 1);
  return {};
}
LuaHelper::Push(*this, luaState);
lua_call(luaState, 1, 1);
auto result = InnerStruct::ReadFromLua(luaState, -1);
lua_pop(luaState, 1);
return result;
  )R");
}

TEST_F(LuaFunctionPluginTest, DuplicateFunction) {
  ExpectErrorMessage(R"R(
struct A {
  @luaFunc
  func TestFunction() -> u32;
  @luaFunc
  func TestFunction() -> u32;
}
  )R",
                     Run, "Duplicate method: A.TestFunction ({0}:3:3) and A.TestFunction ({0}:5:3)",
                     Source);
  ExpectErrorMessage(R"R(
struct A {
  @luaFunc
  func TestFunction(string arg) -> u32;
  @luaFunc
  func TestFunction(string arg) -> u32;
}
  )R",
                     Run, "Duplicate method: A.TestFunction ({0}:3:3) and A.TestFunction ({0}:5:3)",
                     Source);
}

TEST_F(LuaFunctionPluginTest, InvalidType) {
  ExpectErrorMessage("struct A { @luaFunc func TestFunction() -> u63; }", Run,
                     "Unknown type u63 used by A.TestFunction ({0}:1:21)", Source);
  ExpectErrorMessage("struct A { @luaFunc func TestFunction(u33 arg) -> u64; }", Run,
                     "Unknown type u33 used by A.TestFunction ({0}:1:21)", Source);
  ExpectErrorMessage("struct A { @luaFunc func TestFunction(void arg) -> u64; }", Run,
                     "Invalid void usage in A.TestFunction ({0}:1:21)", Source);

  ExpectErrorMessage("struct A { @luaFunc func TestFunction() -> void<u32>; }", Run,
                     "Void cannot have template parameters in A.TestFunction ({0}:1:21)", Source);
}
