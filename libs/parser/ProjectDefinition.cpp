#include "ProjectDefinition.h"
#include "core/Decorators.h"
#include "core/St.h"

namespace holgen {

  const DecoratorDefinition *FieldDefinition::GetDecorator(const std::string &name) const {
    for (const auto &decorator: mDecorators) {
      if (decorator.mName == name)
        return &decorator;
    }
    return nullptr;
  }

  const DecoratorAttributeDefinition *DecoratorDefinition::GetAttribute(const std::string &name) const {
    for (const auto &attribute: mAttributes) {
      if (attribute.mName == name)
        return &attribute;
    }
    return nullptr;
  }

  const DecoratorDefinition *StructDefinition::GetDecorator(const std::string &name) const {
    for (const auto &decorator: mDecorators) {
      if (decorator.mName == name)
        return &decorator;
    }
    return nullptr;
  }

  const FieldDefinition *StructDefinition::GetField(const std::string &name) const {
    for (const auto &field: mFields) {
      if (field.mName == name)
        return &field;
    }
    return nullptr;
  }

  const FieldDefinition *StructDefinition::GetIdField() const {
    for (const auto &field: mFields) {
      if (field.GetDecorator(Decorators::Id))
        return &field;
    }
    return nullptr;
  }

  const StructDefinition *ProjectDefinition::GetStruct(const std::string &name) const {
    for (const auto &structDefinition: mStructs) {
      if (structDefinition.mName == name)
        return &structDefinition;
    }
    return nullptr;
  }

  bool TypeDefinition::operator==(const TypeDefinition &rhs) const {
    if (mName != rhs.mName)
      return false;
    if (mTemplateParameters.size() != rhs.mTemplateParameters.size())
      return false;
    for (size_t i = 0; i < mTemplateParameters.size(); ++i) {
      if (mTemplateParameters[i] != rhs.mTemplateParameters[i])
        return false;
    }
    return true;
  }

}
