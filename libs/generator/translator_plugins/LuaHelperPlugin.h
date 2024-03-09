#pragma once

#include "TranslatorPlugin.h"

namespace holgen {

class LuaHelperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void GenerateHelpers() override;
private:
  void GenerateLuaHelper(Class &generatedClass);
  void GenerateLuaHelperPush(Class &generatedClass);
  void GenerateLuaHelperRead(Class &generatedClass);
  };

}
