#pragma once
#include "../TranslatorPlugin.h"

namespace holgen {
class JsonDumpPlugin  : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  void ProcessStruct(Class &cls);
  void ProcessEnum(Class &cls);
};
} // namespace holgen
