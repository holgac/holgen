#include <gtest/gtest.h>
#include <filesystem>
#include "DotNetHost.h"
#include "SingletonCounter.h"
#include "CounterManager.h"

using namespace holgen_blackbox_csharp;

class ModuleTest : public ::testing::Test {
protected:
  void SetUp() override {
    std::vector<char> pathBuffer(4096, 0);
    getcwd(pathBuffer.data(), pathBuffer.size() - 1);
    mPathToBinFolder = pathBuffer.data();
    mPathToBinFolder = mPathToBinFolder / "bin";
    SingletonCounter::SetSingleton(new SingletonCounter());
    CounterManager::SetSingleton(new CounterManager);
  }

  void TearDown() override {
    SingletonCounter::DeleteSingleton();
    CounterManager::DeleteSingleton();
  }

  std::filesystem::path mPathToBinFolder;
};

TEST_F(ModuleTest, SingletonUsageWithOneModule) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  module1.Initialize();
  auto &counter = SingletonCounter::GetInstance();
  EXPECT_EQ(counter.GetVal(), 0);
  EXPECT_EQ(module1.BumpAndGetSingletonCounter(), 1);
  EXPECT_EQ(counter.GetVal(), 1);
  EXPECT_EQ(module1.BumpAndGetSingletonCounter(), 2);
  EXPECT_EQ(counter.GetVal(), 2);
}

TEST_F(ModuleTest, SingletonUsageWithTwoModules) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  auto &counter = SingletonCounter::GetInstance();
  module1.SetBumpAmount(10);
  EXPECT_EQ(module1.BumpAndGetSingletonCounter(), 10);
  EXPECT_EQ(counter.GetVal(), 10);
  module2.SetBumpAmount(100);
  // module 2 bumps two times the passed amount
  EXPECT_EQ(module2.BumpAndGetSingletonCounter(), 210);
  EXPECT_EQ(counter.GetVal(), 210);
}

TEST_F(ModuleTest, Constructor) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  EXPECT_EQ(module1.Constructor(), ModuleVersion(0, 10, 20));
  EXPECT_EQ(module2.Constructor(), ModuleVersion(30, 40, 50));
}

TEST_F(ModuleTest, MirroredStructManagedToNative) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  EXPECT_EQ(module1.MirroredStructManagedToNative(), ModuleVersion(0, 1, 2));
  EXPECT_EQ(module2.MirroredStructManagedToNative(), ModuleVersion(3, 4, 5));
}

TEST_F(ModuleTest, MirroredStructNativeToManagedToNative) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  EXPECT_EQ(module1.MirroredStructNativeToManagedToNative(), ModuleVersion(0, 2, 4));
  EXPECT_EQ(module2.MirroredStructNativeToManagedToNative(), ModuleVersion(6, 8, 10));
}

TEST_F(ModuleTest, MirroredStructMethodCall) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  EXPECT_EQ(module1.MirroredStructMethodCall(), ModuleVersion(0, 1, 3));
  EXPECT_EQ(module2.MirroredStructMethodCall(), ModuleVersion(3, 4, 12));
}

TEST_F(ModuleTest, MirroredStructInArguments) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  EXPECT_EQ(module1.MirroredStructInArguments(), ModuleVersion(0, 111, 222));
  EXPECT_EQ(module2.MirroredStructInArguments(), ModuleVersion(333, 444, 555));
}

TEST_F(ModuleTest, ProxyObjectNativeToManaged) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  auto &counterManager = CounterManager::GetInstance();
  auto &counters = counterManager.GetCounters();
  EXPECT_TRUE(counters.empty());
  EXPECT_EQ(module1.ProxyObjectNativeToManaged(), 1);
  EXPECT_EQ(counters.size(), 1);
  EXPECT_EQ(module1.ProxyObjectNativeToManaged(), 2);
  EXPECT_EQ(counters.size(), 1);
  ASSERT_NE(counters.find("TestModule"), counters.end());
  EXPECT_EQ(counters.at("TestModule").Get(), 2);

  module1.SetCounterName("TestModuleCounter2");
  EXPECT_EQ(module1.ProxyObjectNativeToManaged(), 1);
  ASSERT_NE(counters.find("TestModuleCounter2"), counters.end());
  EXPECT_EQ(counters.at("TestModuleCounter2").Get(), 1);

  // module2 bumps 2 times
  EXPECT_EQ(module2.ProxyObjectNativeToManaged(), 2);
  ASSERT_NE(counters.find("TestModule2"), counters.end());
  EXPECT_EQ(counters.at("TestModule2").Get(), 2);
}

TEST_F(ModuleTest, ProxyObjectMethodArg) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  auto &counterManager = CounterManager::GetInstance();
  counterManager.GetCounter("TestCounter").Bump(123);
  EXPECT_EQ(module1.ProxyObjectMethodArgInModule(counterManager.GetCounterPtr("TestCounter")), 123);
  EXPECT_EQ(module2.ProxyObjectMethodArgInModule(counterManager.GetCounterPtr("TestCounter")), 246);
}

TEST_F(ModuleTest, ProxyObjectReturnValue) {
  DotNetHost mDotNetHost;
  mDotNetHost.Initialize(mPathToBinFolder / "CSharpBindings");
  auto &module1 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule");
  auto &module2 = mDotNetHost.LoadCustomDotNetModule(mPathToBinFolder / "TestModule2");
  module1.Initialize();
  module2.Initialize();
  auto &counterManager = CounterManager::GetInstance();
  auto c1 = counterManager.GetCounterPtr("TestCounter1");
  auto c2 = counterManager.GetCounterPtr("TestCounter2");

  c1->Bump(10);
  c2->Bump(100);
  EXPECT_EQ(module1.ProxyObjectReturnValue(c1, c2), c1);
  EXPECT_EQ(c1->Get(), 110);
  c1->Reset();
  c2->Reset();

  c1->Bump(10);
  c2->Bump(100);
  EXPECT_EQ(module2.ProxyObjectReturnValue(c1, c2), c1);
  EXPECT_EQ(c1->Get(), 210);
}
