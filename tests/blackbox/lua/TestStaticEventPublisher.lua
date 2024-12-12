-- This file is generated by holgen. Do not modify manually.
---@class TestStaticEventPublisher
TestStaticEventPublisher = {
  ---@type [string, TestEventListener]
  Modules = {},
  ---@type [TestEventListener]
  OnEvent1_Callbacks = {},
  ---@type [TestEventListener]
  OnEvent2_Callbacks = {},

  ---@param val number
  OnEvent1 = function(val)
    for _, module in pairs(TestStaticEventPublisher.OnEvent1_Callbacks) do
      module:OnEvent1(val)
    end
  end,
  OnEvent2 = function()
    ---@type [string, number]
    local retVal = {}
    for _, module in pairs(TestStaticEventPublisher.OnEvent2_Callbacks) do
      retVal[module.name] = module:OnEvent2()
    end
    return retVal
  end,

  ---@param subscriber TestEventListener
  RegisterSubscriber = function(subscriber)
    if subscriber.name and subscriber.name ~= '' then
      TestStaticEventPublisher.Modules[subscriber.name] = subscriber
    end
    if subscriber.OnEvent1 then
      table.insert(TestStaticEventPublisher.OnEvent1_Callbacks, subscriber)
    end
    if subscriber.OnEvent2 then
      table.insert(TestStaticEventPublisher.OnEvent2_Callbacks, subscriber)
    end
  end,

  ---@param subscriber TestEventListener
  UnregisterSubscriber = function(subscriber)
    if subscriber.name and subscriber.name ~= '' then
      TestStaticEventPublisher.Modules[subscriber.name] = nil
    end
    if subscriber.OnEvent1 then
      TestStaticEventPublisher.UnsubscribeFromEvent(TestStaticEventPublisher.OnEvent1_Callbacks, subscriber)
    end
    if subscriber.OnEvent2 then
      TestStaticEventPublisher.UnsubscribeFromEvent(TestStaticEventPublisher.OnEvent2_Callbacks, subscriber)
    end
  end,

  ---@param subscriberName string
  UnregisterSubscriberByName = function(subscriberName)
    if Modules[subscriberName] then
      TestStaticEventPublisher.UnregisterSubscriber(Modules[subscriberName])
    end
  end,

  ClearSubscribers = function(subscriberClass)
    TestStaticEventPublisher.Modules = {}
    TestStaticEventPublisher.OnEvent1_Callbacks = {}
    TestStaticEventPublisher.OnEvent2_Callbacks = {}
  end,

  ---@param subscriberClass TestEventListener
  ---@return TestEventListener
  ReloadSubscriber = function(subscriberClass)
    local oldInstance = TestStaticEventPublisher.Modules[subscriberClass.name]
    if oldInstance then
      TestStaticEventPublisher.UnregisterSubscriber(oldInstance)
    end
    local newInstance = subscriberClass:new(oldInstance)
    TestStaticEventPublisher.RegisterSubscriber(newInstance)
    return newInstance
  end,

  ---@param eventTable [TestEventListener]
  ---@param subscriber TestEventListener
  SubscribeToEvent = function(eventTable, subscriber)
    TestStaticEventPublisher.UnsubscribeFromEvent(eventTable, subscriber)
    table.insert(eventTable, subscriber)
  end,

  ---@param eventTable [TestEventListener]
  ---@param subscriber TestEventListener
  ---@return boolean
  UnsubscribeFromEvent = function(eventTable, subscriber)
    for i = 1, #eventTable do
      if eventTable[i] == subscriber then
        table.remove(eventTable, i)
        return true
      end
    end
    return false
  end

}