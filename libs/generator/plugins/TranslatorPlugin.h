#pragma once

#include "../NamingConvention.h"
#include "../TranslatedProject.h"
#include "../TranslatorSettings.h"
#include "../Validator.h"
#include "core/Annotations.h"

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

  template <typename T, typename C>
  void FillComments(const T &definition, C &container) {
    for (auto &annotation: definition.GetAnnotations(Annotations::Comment)) {
      for (auto &attribute: annotation.mAttributes) {
        container.push_back(attribute.mName);
      }
    }
  }

private:
  // TODO(RELEASE): get this from TranslatorSettings
  NamingConvention mNamingConvention;
  Validator mValidator;
};
} // namespace holgen
