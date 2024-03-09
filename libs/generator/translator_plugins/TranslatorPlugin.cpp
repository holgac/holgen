#include "TranslatorPlugin.h"

namespace holgen {

  TranslatorPlugin::TranslatorPlugin(
      TranslatedProject &translatedProject
  ) : mProject(translatedProject) {
  }

  TranslatorPlugin::~TranslatorPlugin() {}

  void TranslatorPlugin::Run() {
  }
}
