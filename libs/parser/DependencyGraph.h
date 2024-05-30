#pragma once

#include <map>
#include <string>
#include <set>
#include "ProjectDefinition.h"

namespace holgen {

class DependencyGraph {
public:
  explicit DependencyGraph(const ProjectDefinition &project);
  const std::vector<std::string> &GetProcessOrder() const;
private:
  void Calculate();
  void Calculate(const StructDefinition &structDefinition, const TypeDefinition &typeDefinition);
  void Calculate(const StructDefinition &structDefinition, const FieldDefinition &fieldDefinition);
  void Calculate(const StructDefinition &structDefinition);
  const ProjectDefinition &mProject;
  std::map<std::string, std::set<std::string>> mDependencies;
  std::map<std::string, std::set<std::string>> mInverseDependencies;
  std::vector<std::string> mProcessOrder;
};
}

