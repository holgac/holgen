#pragma once

#include "../TranslatedProject.h"
#include "../NamingConvention.h"
#include "../Validator.h"
#include "../TranslatorSettings.h"

namespace holgen {
  class TranslatorPlugin {
  public:
    TranslatorPlugin(TranslatedProject &translatedProject, const TranslatorSettings &translatorSettings);
    virtual ~TranslatorPlugin();
    virtual void Run();
    const NamingConvention &Naming() const;
    const Validator &Validate() const;
  protected:
    TranslatedProject &mProject;
    const TranslatorSettings &mSettings;
  private:
    // TODO(RELEASE): get this from TranslatorSettings
    NamingConvention mNamingConvention;
    Validator mValidator;
  };
}
