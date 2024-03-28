#pragma once

#include "TranslatorPlugin.h"

namespace holgen {
  class ContainerFieldPlugin : public TranslatorPlugin {
  public:
    using TranslatorPlugin::TranslatorPlugin;
     void Run() override;
  private:
    void ProcessContainerField(Class& generatedClass, const FieldDefinition& fieldDefinition);
    void ProcessContainerIndex(Class& generatedClass, const FieldDefinition& fieldDefinition, const AnnotationDefinition& annotationDefinition);
    void GenerateContainerNextIndexField(Class& generatedClass, const FieldDefinition& fieldDefinition);
    void GenerateContainerAddElem(Class& generatedClass, const FieldDefinition& fieldDefinition, bool useMoveRef);
    void GenerateContainerHasElem(Class& generatedClass, const FieldDefinition& fieldDefinition);
    void GenerateContainerGetElem(Class& generatedClass, const FieldDefinition& fieldDefinition);
    void GenerateContainerGetCount(Class& generatedClass, const FieldDefinition& fieldDefinition);
    void GenerateContainerDeleteElem(Class& generatedClass, const FieldDefinition& fieldDefinition);
    bool CanImplementHasElem(Class& generatedClass, const FieldDefinition& fieldDefinition);
    bool CanImplementDeleteElem(Class& generatedClass, const FieldDefinition& fieldDefinition);

  };
}
