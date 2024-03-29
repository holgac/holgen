#pragma once

#include "TranslatedProject.h"

namespace holgen {
  class Validator {
  public:
    explicit Validator(TranslatedProject &project);
    void NewClass(const Class &cls) const;
  private:
    TranslatedProject &mProject;
  };
}