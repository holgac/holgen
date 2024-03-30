#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassPlugin.h"
#include "generator/plugins/CppFunctionPlugin.h"

class CppFunctionPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project).Run();
    CppFunctionPlugin(project).Run();
  }
};

TEST_F(CppFunctionPluginTest, FunctionWithArgs) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  @cppFunc
  func TestFunction(s32 a1, string a2, InnerStruct a3) -> string;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"TestFunction", Type{"std::string"}, Visibility::Public, Constness::NotConst};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("a1", Type{"int32_t"});
  method.mArguments.emplace_back("a2", Type{"std::string", PassByType::Reference, Constness::Const});
  method.mArguments.emplace_back("a3", Type{"InnerStruct", PassByType::Pointer, Constness::Const});
  method.mUserDefined = true;
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::NotConst), method);
}

TEST_F(CppFunctionPluginTest, FunctionWithOutArgs) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  @cppFunc
  func TestFunction(s32 a1 out, string a2 out, InnerStruct a3 out) -> string;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"TestFunction", Type{"std::string"}, Visibility::Public, Constness::NotConst};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("a1", Type{"int32_t", PassByType::Reference});
  method.mArguments.emplace_back("a2", Type{"std::string", PassByType::Reference});
  method.mArguments.emplace_back("a3", Type{"InnerStruct", PassByType::Pointer});
  method.mUserDefined = true;
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::NotConst), method);
}

TEST_F(CppFunctionPluginTest, FunctionReturningStruct) {
  auto project = Parse(R"R(
struct InnerStruct {}
struct TestData {
  @cppFunc
  func TestFunction(s32 a1) -> InnerStruct;
}
  )R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  auto method = ClassMethod{"TestFunction", Type{"InnerStruct", PassByType::Pointer}, Visibility::Public, Constness::NotConst};
  method.mFunction = cls->mStruct->GetFunction("TestFunction");
  method.mArguments.emplace_back("a1", Type{"int32_t"});
  method.mUserDefined = true;
  method.mExposeToLua = true;
  helpers::ExpectEqual(*cls->GetMethod("TestFunction", Constness::NotConst), method);
}
