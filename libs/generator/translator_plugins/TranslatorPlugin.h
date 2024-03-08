#pragma once

#include "../TranslatedProject.h"


namespace holgen {
  class TranslatorPlugin {
  public:
    TranslatorPlugin(TranslatedProject &translatedProject);
    virtual ~TranslatorPlugin();
    virtual void EnrichClasses();
    virtual void GenerateHelpers();
  protected:
    TranslatedProject &mProject;
  };

}
