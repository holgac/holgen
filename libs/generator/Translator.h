#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "core/LineWithAction.h"
#include "TranslatedProject.h"
#include "plugins/TranslatorPlugin.h"
#include "TranslatorSettings.h"

namespace holgen {
  class Translator {
  public:
    Translator(const TranslatorSettings &translatorSettings);

    template<typename Plugin, typename ...Args>
    void AddPlugin(Args &&... args) {
      // mPlugins.push_back(std::move(std::make_unique<Plugin>(mProject, std::forward<Args>(args)...)));
      mPlugins.push_back([&](TranslatedProject &project) {
        Plugin(project, mTranslatorSettings, std::forward<Args>(args)...).Run();
      });
    }

    TranslatedProject Translate(const ProjectDefinition &project) const;
  private:
    // std::vector<std::unique_ptr<TranslatorPlugin>> mPlugins;
    std::vector<std::function<void(TranslatedProject &)>> mPlugins;
    TranslatorSettings mTranslatorSettings;
  };
}
