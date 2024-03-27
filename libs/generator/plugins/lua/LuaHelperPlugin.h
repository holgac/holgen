#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class LuaHelperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  // TODO: LuaHelper in name not necessary
  void GenerateLuaHelperPush(Class &generatedClass);
  void GenerateLuaHelperRead(Class &generatedClass);
  void GenerateLuaHelperPushNil(Class &generatedClass);
  void GenerateCreateMetatables(Class &generatedClass);
  void GeneratePushForSingleElemContainer(Class &cls, const std::string& container);
  void GenerateReadForSingleElemContainer(Class &cls, const std::string& container);
};

}
