#pragma once

#include <vector>
#include <string>
#include <memory>
#include "core/LineWithAction.h"
#include "TranslatedProject.h"
#include "plugins/TranslatorPlugin.h"

namespace holgen {
  class Translator {
  public:

    Translator(const ProjectDefinition &project);

    template<typename Plugin, typename ...Args>
    void AddPlugin(Args &&... args) {
      mPlugins.push_back(std::move(std::make_unique<Plugin>(mProject, std::forward<Args>(args)...)));
    }

    TranslatedProject Translate();
  private:
    TranslatedProject mProject;
    std::vector<std::unique_ptr<TranslatorPlugin>> mPlugins;
  };
}
