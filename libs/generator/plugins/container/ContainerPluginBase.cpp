#include "ContainerPluginBase.h"

namespace holgen {
bool ContainerPluginBase::ShouldProcess(const ClassField &field) const {
  if (!field.mField || !field.mField->GetAnnotation(Annotations::Container))
    return false;
  return true;
}
} // namespace holgen
