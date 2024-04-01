#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

  class LuaPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;
  private:
    void ProcessStruct(Class &cls);
    void ProcessEnum(Class &cls);
    void GeneratePushGlobalToLua(Class &cls);
    void GeneratePushToLua(Class &cls);
    void GenerateReadFromLua(Class &cls);
    void GeneratePushIndexMetaMethod(Class &cls);
    void GeneratePushNewIndexMetaMethod(Class &cls);
    void GenerateCreateLuaMetatable(Class &cls);
  };

}
