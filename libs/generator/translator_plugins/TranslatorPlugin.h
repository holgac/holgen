#pragma once

#include "../TranslatedProject.h"


namespace holgen {
  class TranslatorPlugin {
  public:
    TranslatorPlugin(TranslatedProject &translatedProject);
    virtual ~TranslatorPlugin();
    virtual void Run();
  protected:
    TranslatedProject &mProject;
  };

}
