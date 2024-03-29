#pragma once

#include "TranslatedProject.h"

namespace holgen {
  class Validator {
  public:
    explicit Validator(TranslatedProject &project);
    void NewClass(const Class &cls) const;
    void NewField(const Class &cls, const ClassField &field) const;
    void RefField(const Class &cls, const FieldDefinition &fieldDefinition) const;
  private:
    void ValidateType(const Type &type, const std::string &source) const;
    TranslatedProject &mProject;
  };
}