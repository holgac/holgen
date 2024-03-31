#pragma once

#include <map>
#include <set>
#include <string>
#include "../TranslatorPlugin.h"

namespace holgen {
  class JsonPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;
  private:
    void ProcessStruct(Class &cls);
    void ProcessEnum(Class &cls);
    void GenerateParseJson(Class &cls);
    void GenerateParseJsonForField(ClassMethod &method, const ClassField &field);
    void GenerateParseJsonForFunction(ClassMethod &method, const ClassMethod &luaFunction);
  };
}