#pragma once

#include "TranslatedProject.h"

namespace holgen {
  class Validator {
  public:
    explicit Validator(TranslatedProject &project);
    void NewClass(const Class &cls) const;
    void NewField(const Class &cls, const ClassField &field) const;
    void RefField(const Class &cls, const FieldDefinition &fieldDefinition) const;
    void NewMethod(const Class &cls, const ClassMethod& method) const;
  private:
    void ValidateType(const Type &type, const Class& cls, bool acceptVoid, const std::string &source) const;
    TranslatedProject &mProject;
  };
}