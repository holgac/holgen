#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "TranslatedProject.h"
#include "TranslatorSettings.h"
#include "core/LineWithAction.h"
#include "plugins/TranslatorPlugin.h"

namespace holgen {
class Translator {
public:
  explicit Translator(const TranslatorSettings &translatorSettings);

  template <typename Plugin, typename... Args>
  void AddPlugin(Args &&...args) {
    mPlugins.push_back(
        [&](TranslatedProject &project) { Plugin(project, mTranslatorSettings, std::forward<Args>(args)...).Run(); });
  }

  [[nodiscard]] TranslatedProject Translate(const ProjectDefinition &project) const;

private:
  // std::vector<std::unique_ptr<TranslatorPlugin>> mPlugins;
  std::vector<std::function<void(TranslatedProject &)>> mPlugins;
  TranslatorSettings mTranslatorSettings;
};
} // namespace holgen
