#include "TranslatorPluginTest.h"
#include "generator/plugins/ClassEqualsOperatorPlugin.h"
#include "generator/plugins/ClassFieldPlugin.h"
#include "generator/plugins/ClassPlugin.h"

class ClassFieldEqualsOperatorPluginTest : public TranslatorPluginTest {
protected:
  static void Run(TranslatedProject &project) {
    ClassPlugin(project, {}).Run();
    ClassFieldPlugin(project, {}).Run();
    ClassEqualsOperatorPlugin(project, {}).Run();
  }
};

TEST_F(ClassFieldEqualsOperatorPluginTest, Operator) {
  auto project = Parse(R"R(
struct InnerData {}
@epsilon('0.001')
struct TestData {
  u32 testFieldUnsigned = 42;
  @epsilon('0.000001')
  double testFieldDouble;
  float testFieldFloat;
  InnerData  testFieldInner;
})R");
  Run(project);
  auto cls = project.GetClass("TestData");
  ASSERT_NE(cls, nullptr);

  ASSERT_NE(cls->GetMethod("operator==", Constness::Const), nullptr);
  {
    auto method = ClassMethod{"operator==", Type{"bool"}, Visibility::Public, Constness::Const};
    method.mArguments.emplace_back("rhs",
                                   Type{"TestData", PassByType::Reference, Constness::Const});
    helpers::ExpectEqual(*cls->GetMethod("operator==", Constness::Const), method,
                         R"R(
return !(
    mTestFieldUnsigned != rhs.mTestFieldUnsigned ||
    std::fabs(mTestFieldDouble - rhs.mTestFieldDouble) >= 0.000001 ||
    std::fabs(mTestFieldFloat - rhs.mTestFieldFloat) >= 0.001 ||
    mTestFieldInner != rhs.mTestFieldInner
);
)R");
  }
}
