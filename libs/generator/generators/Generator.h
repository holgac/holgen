#pragma once


#pragma once

#include "parser/Parser.h"
#include "../Translator.h"

namespace holgen {
  class Generator {
  public:
    Generator(const ProjectDefinition &projectDefinition, TranslatedProject &translatedProject);
    virtual ~Generator();
    virtual void EnrichClasses();
    virtual void GenerateHelpers();
  protected:
    const ProjectDefinition &mProjectDefinition;
    TranslatedProject &mTranslatedProject;
  };

}
