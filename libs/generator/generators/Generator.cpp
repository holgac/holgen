#include "Generator.h"

namespace holgen {

  Generator::Generator(
      const ProjectDefinition &projectDefinition, TranslatedProject &translatedProject
  ) : mProjectDefinition(projectDefinition), mTranslatedProject(translatedProject) {
  }

  Generator::~Generator() {}

  void Generator::GenerateHelpers() {
  }

  void Generator::EnrichClasses() {
  }
}
