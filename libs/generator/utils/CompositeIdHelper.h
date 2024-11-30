#pragma once
#include "generator/types/CppTypes.h"

namespace holgen {
class CompositeIdHelper {
public:
  static const ClassField* GetObjectTypeField(const Class& cls);
  static const ClassField* GetObjectIdField(const Class& cls, bool throwOnFailure = true);
  static const ClassField* GetObjectVersionField(const Class& cls);
  static const ClassField* GetIdTypeField(const Class& cls);
  static const ClassField* GetIdIdField(const Class& cls);
  static const ClassField* GetIdVersionField(const Class& cls);
  static const Class* GetCompositeIdType(const TranslatedProject& project, const Class& cls);
private:
  static const ClassField* GetIdFieldWithAttribute(const Class& cls, const std::string& attribute, bool throwOnFailure);
  static const ClassField* GetObjectFieldWithAttribute(const Class& cls, const std::string& attribute, bool throwOnFailure);
  static const ClassField* GetFieldWithAttribute(const Class& cls, const std::string& annotation, const std::string& attribute, bool throwOnFailure);
};
}
