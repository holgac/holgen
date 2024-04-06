#pragma once
#include "holgen.h"
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
    void GenerateIndexMetaMethod(Class &cls);
    void GenerateNewIndexMetaMethod(Class &cls);
    void GenerateCreateLuaMetatable(Class &cls);
    bool ShouldEmbedPointer(Class& cls);
  };

}
