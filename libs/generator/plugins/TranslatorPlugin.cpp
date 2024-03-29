#include "TranslatorPlugin.h"

namespace holgen {

  TranslatorPlugin::TranslatorPlugin(
      TranslatedProject &translatedProject
  ) : mProject(translatedProject), mNamingConvention(mProject), mValidator(mProject) {
  }

  TranslatorPlugin::~TranslatorPlugin() {}

  void TranslatorPlugin::Run() {
  }

  const NamingConvention &TranslatorPlugin::Naming() const {
    return mNamingConvention;
  }

  const Validator &TranslatorPlugin::Validate() const {
    return mValidator;
  }
}
