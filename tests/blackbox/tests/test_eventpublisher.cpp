#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TestEventListener.h"
#include "TestStaticEventPublisher.h"
#include "LuaTestHelper.h"

#include <LuaHelper.h>

using namespace holgen_blackbox_test;

class EventPublisherTest : public ::testing::Test {
protected:
  void SetUp() override {
    mState.Init();
    LuaHelper::Setup(mState, "lua");
  }

  void TearDown() override {
    mState.Destroy();
  }

  void GenerateTestListener(const std::string &listenerName, const std::string &counterName) {
    auto luaCode = std::format(

        R"(
{1} = 0
{0} = {{
  OnEvent1 = function(self, val)
    {1} = {1} + val
  end
}}
)",
        listenerName, counterName);
    luaL_dostring(mState, luaCode.c_str());
  }

  void ExpectCounter(const std::string &counterName, int value) {
    lua_getglobal(mState, counterName.c_str());
    LuaTestHelper::ExpectStack(mState, {std::to_string(value)});
    lua_pop(mState, 1);
  }

  LuaState mState;
};

TEST_F(EventPublisherTest, PublishEvent) {
  GenerateTestListener("TestListener", "Counter");
  ExpectCounter("Counter", 0);
  TestStaticEventPublisher::OnEvent1(mState, 10);
  ExpectCounter("Counter", 0);
  TestStaticEventPublisher::RegisterSubscriber(
      mState, [](lua_State *luaState) { lua_getglobal(luaState, "TestListener"); });
  TestStaticEventPublisher::OnEvent1(mState, 10);
  ExpectCounter("Counter", 10);
}

TEST_F(EventPublisherTest, ClearListeners) {
  GenerateTestListener("TestListener", "Counter");
  TestStaticEventPublisher::RegisterSubscriber(
      mState, [](lua_State *luaState) { lua_getglobal(luaState, "TestListener"); });
  TestStaticEventPublisher::ClearSubscribers(mState);
  TestStaticEventPublisher::OnEvent1(mState, 10);
  ExpectCounter("Counter", 0);
}
