#pragma once
#include "generator/types/CppTypes.h"

namespace holgen {
class CompositeIdHelper {
public:
  static const ClassField* GetTypeField(const Class& cls);
  static const ClassField* GetIdField(const Class& cls);
  static const ClassField* GetVersionField(const Class& cls);
private:
  static const ClassField* GetFieldWithAttribute(const Class& cls, const std::string& attribute, bool throwOnFailure);
};
}
