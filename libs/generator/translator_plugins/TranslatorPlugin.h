#pragma once

#include "../TranslatedProject.h"


namespace holgen {
  class TranslatorPlugin {
  public:
    TranslatorPlugin(TranslatedProject &translatedProject);
    virtual ~TranslatorPlugin();
    // TODO: just a Run function
    virtual void EnrichClasses();
    virtual void GenerateHelpers();
  protected:
    TranslatedProject &mProject;
  };

}
