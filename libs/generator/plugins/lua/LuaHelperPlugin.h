#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class LuaHelperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;

private:
  void GeneratePush(Class &cls);
  void GenerateBasePush(Class &cls);
  void GeneratePushNil(Class &cls);
  void GeneratePushForSmartPointer(Class &cls, const std::string &pointerType);
  void GeneratePushForSingleElemContainer(Class &cls, const std::string &container);
  void GenerateReadForSingleElemContainer(Class &cls, const std::string &container);
  void GeneratePushForPrimitives(Class &cls);
  void GeneratePushForContainers(Class &cls);
  void GeneratePushTuple(Class &cls, size_t size, const std::string &tupleClassName);
  void GenerateRead(Class &cls);
  void GenerateBaseRead(Class &cls);
  void GenerateReadForPrimitives(Class &cls);
  void GenerateReadForContainers(Class &cls);
  void GenerateReadFunction(Class &cls);
  void GenerateReadTuple(Class &cls, size_t size, const std::string &tupleClassName);
  void GenerateInitializeLua(Class &cls);
  void GeneratePushForKeyedContainer(Class &cls, const std::string &container) const;
  void GenerateInitializeClasses(Class &cls, ClassMethod &method);
  void GenerateInitializeFunctionTables(ClassMethod &method);
};

} // namespace holgen
