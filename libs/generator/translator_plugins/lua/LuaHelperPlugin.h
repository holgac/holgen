#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class LuaHelperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void GenerateLuaHelperPush(Class &generatedClass);
  void GenerateLuaHelperRead(Class &generatedClass);
  void GenerateLuaHelperPushNil(Class &generatedClass);
  void GenerateLuaHelperPushPtr(Class &generatedClass);
};

}
