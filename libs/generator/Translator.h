#pragma once

#include <vector>
#include <string>
#include <memory>
#include "core/LineWithAction.h"
// Needed for Project (TODO: move to separate file and fwd declare)
#include "parser/Parser.h"
#include "TranslatedProject.h"
#include "translator_plugins/TranslatorPlugin.h"
// TODO: register custom generators
// #include "generators/Generator.h"


namespace holgen {
  class Translator {
  public:

    Translator(const ProjectDefinition &project);

    template<typename Plugin, typename ...Args>
    void AddPlugin(Args &&... args) {
      mPlugins.emplace_back(std::move(std::make_unique<Plugin>(mProject, std::forward<Args>(args)...)));
    }

    TranslatedProject Translate();
  private:
    TranslatedProject mProject;
    std::vector<std::unique_ptr<TranslatorPlugin>> mPlugins;
  };
}
