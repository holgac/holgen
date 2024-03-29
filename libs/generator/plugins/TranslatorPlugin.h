#pragma once

#include "../TranslatedProject.h"
#include "../NamingConvention.h"
#include "../Validator.h"


namespace holgen {
  class TranslatorPlugin {
  public:
    TranslatorPlugin(TranslatedProject &translatedProject);
    virtual ~TranslatorPlugin();
    virtual void Run();
    const NamingConvention &Naming() const;
    const Validator &Validate() const;
  protected:
    TranslatedProject &mProject;
  private:
    // TODO(RELEASE): get this from GeneratorSettings
    NamingConvention mNamingConvention;
    Validator mValidator;
  };
}
