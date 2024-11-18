#pragma once

#include "../TranslatorPlugin.h"

namespace holgen {

class FilesystemHelperPlugin : public TranslatorPlugin {
public:
  using TranslatorPlugin::TranslatorPlugin;
  void Run() override;
private:
  Class& GenerateClass() const;
  void GenerateReadFile(Class& cls) const;
  void GenerateDumpFile(Class& cls) const;
};

} // namespace holgen
