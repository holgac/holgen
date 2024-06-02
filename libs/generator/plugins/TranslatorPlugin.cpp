#include "TranslatorPlugin.h"

namespace holgen {

TranslatorPlugin::TranslatorPlugin(TranslatedProject &translatedProject,
                                   const TranslatorSettings &translatorSettings) :
    mProject(translatedProject), mSettings(translatorSettings), mNamingConvention(mProject),
    mValidator(mProject, mNamingConvention) {}

TranslatorPlugin::~TranslatorPlugin() {}

void TranslatorPlugin::Run() {}

const NamingConvention &TranslatorPlugin::Naming() const {
  return mNamingConvention;
}

const Validator &TranslatorPlugin::Validate() const {
  return mValidator;
}
} // namespace holgen
