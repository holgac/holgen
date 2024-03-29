#include "TranslatorPlugin.h"

namespace holgen {

  TranslatorPlugin::TranslatorPlugin(
      TranslatedProject &translatedProject
  ) : mProject(translatedProject), mNamingConvention(mProject) {
  }

  TranslatorPlugin::~TranslatorPlugin() {}

  void TranslatorPlugin::Run() {
  }

  const NamingConvention &TranslatorPlugin::Naming() const {
    return mNamingConvention;
  }
}
