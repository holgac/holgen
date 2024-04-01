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
    void GeneratePushGlobalToLua(Class &generatedClass);
    void GeneratePushToLua(Class &generatedClass);
    void GenerateReadFromLua(Class &generatedClass);
    void GeneratePushIndexMetaMethod(Class &cls);
    void GeneratePushNewIndexMetaMethod(Class &cls);
    void GenerateCreateLuaMetatable(Class &cls);
  };

}
