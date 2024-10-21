// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <rapidjson/fwd.h>
#include "TestStructVirtualMethods.h"

struct lua_State;
namespace holgen_blackbox_test {
  class Converter;
}
namespace holgen_blackbox_test {
// Defined in tests/blackbox/schemas/teststruct.hsc
class TestStructSmartPointers {
public:
  TestStructSmartPointers() = default;
  TestStructSmartPointers(const TestStructSmartPointers &rhs) = delete;
  TestStructSmartPointers(TestStructSmartPointers &&rhs) noexcept;
  ~TestStructSmartPointers() = default;
  bool operator==(const TestStructSmartPointers &rhs) const;
  const std::shared_ptr<TestStructVirtualMethods> &GetSharedPtr() const;
  std::shared_ptr<TestStructVirtualMethods> &GetSharedPtr();
  const std::vector<std::shared_ptr<TestStructVirtualMethods>> &GetSharedPtrs() const;
  std::vector<std::shared_ptr<TestStructVirtualMethods>> &GetSharedPtrs();
  const std::unique_ptr<TestStructVirtualMethods> &GetUniquePtr() const;
  std::unique_ptr<TestStructVirtualMethods> &GetUniquePtr();
  const std::vector<std::unique_ptr<TestStructVirtualMethods>> &GetUniquePtrs() const;
  std::vector<std::unique_ptr<TestStructVirtualMethods>> &GetUniquePtrs();
  void SetSharedPtr(const std::shared_ptr<TestStructVirtualMethods> &val);
  void SetSharedPtrs(const std::vector<std::shared_ptr<TestStructVirtualMethods>> &val);
  void SetUniquePtr(std::unique_ptr<TestStructVirtualMethods> &&val);
  void SetUniquePtrs(std::vector<std::unique_ptr<TestStructVirtualMethods>> &&val);
  bool ParseJson(const rapidjson::Value &json, const Converter &converter);
  void PushToLua(lua_State *luaState) const;
  void PushGlobalToLua(lua_State *luaState, const char *name) const;
  /*
   * This only works with negative indices
   * Reads proxy object (a table with a metatable and an embedded pointer or an index)
   */
  static TestStructSmartPointers *ReadProxyFromLua(lua_State *luaState, int32_t idx);
  static void CreateLuaMetatable(lua_State *luaState);
  TestStructSmartPointers &operator=(const TestStructSmartPointers &rhs) = delete;
  TestStructSmartPointers &operator=(TestStructSmartPointers &&rhs) noexcept;
  inline static const char *CLASS_NAME = "TestStructSmartPointers";
private:
  static int NewIndexMetaMethod(lua_State *luaState);
  static int IndexMetaMethod(lua_State *luaState);
  std::shared_ptr<TestStructVirtualMethods> mSharedPtr;
  std::vector<std::shared_ptr<TestStructVirtualMethods>> mSharedPtrs;
  std::unique_ptr<TestStructVirtualMethods> mUniquePtr;
  std::vector<std::unique_ptr<TestStructVirtualMethods>> mUniquePtrs;
};
}
