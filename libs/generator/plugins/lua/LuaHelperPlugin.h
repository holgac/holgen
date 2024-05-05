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
    void GeneratePushForSingleElemContainer(Class &cls, const std::string &container);
    void GenerateReadForSingleElemContainer(Class &cls, const std::string &container);
    void GeneratePushForPrimitives(Class &cls);
    void GeneratePushForContainers(Class &cls);
    void GenerateRead(Class &cls);
    void GenerateBaseRead(Class &cls);
    void GenerateReadForPrimitives(Class &cls);
    void GenerateReadForContainers(Class &cls);
    void GenerateCreateMetatables(Class &cls);
    void GeneratePushForKeyedContainer(Class &cls, const std::string &container) const;
  };

}
