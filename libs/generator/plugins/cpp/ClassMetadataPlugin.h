#pragma once
#include "../TranslatorPlugin.h"

namespace holgen
{
  class ClassMetadataPlugin : public TranslatorPlugin
  {
  public:
    using TranslatorPlugin::TranslatorPlugin;
    void Run() override;

  private:
    void Process(Class& cls);
  };
}
