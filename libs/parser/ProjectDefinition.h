#pragma once

#include <string>
#include <vector>


namespace holgen {
  struct TypeDefinition {
    std::string mName;
    std::vector<TypeDefinition> mTemplateParameters;
    bool operator==(const TypeDefinition &rhs) const;
  };

  // @decorator(attribute=5)
  struct DecoratorAttributeDefinition {
    std::string mName;
    TypeDefinition mValue;
  };

  // TODO: This is not a decorator, find a better name
  struct DecoratorDefinition {
    std::string mName;
    std::vector<DecoratorAttributeDefinition> mAttributes;
    const DecoratorAttributeDefinition *GetAttribute(const std::string &name) const;
  };

  struct FieldDefinition {
    TypeDefinition mType;
    std::string mName;
    std::vector<DecoratorDefinition> mDecorators;
    const DecoratorDefinition *GetDecorator(const std::string &name) const;
  };

  struct StructDefinition {
    std::string mName;
    std::vector<FieldDefinition> mFields;
    std::vector<DecoratorDefinition> mDecorators;
    const DecoratorDefinition *GetDecorator(const std::string &name) const;
    const FieldDefinition *GetField(const std::string &name) const;
    const FieldDefinition *GetIdField() const;
  };

  struct ProjectDefinition {
    std::vector<StructDefinition> mStructs;
    const StructDefinition *GetStruct(const std::string &name) const;
  };


}