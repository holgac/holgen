#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {
  class LuaFunctionPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  private:
    void AddLuaFunction(Class& cls, const FunctionDefinition& functionDefinition);
    void AddLuaFunctionPushArgs(Class& cls, ClassMethod& method, const FunctionDefinition& functionDefinition);
  };
}
