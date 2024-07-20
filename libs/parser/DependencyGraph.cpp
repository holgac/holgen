#include "DependencyGraph.h"
#include <algorithm>
#include <queue>
#include "core/Annotations.h"
#include "core/Exception.h"
#include "generator/TypeInfo.h"

namespace holgen {
namespace {
std::vector<std::string> AnnotationsToCheck = {
    Annotations::JsonConvert,
};
}

DependencyGraph::DependencyGraph(const ProjectDefinition &project) : mProject(project) {
  Calculate();
}

void DependencyGraph::Calculate() {
  for (const auto &structDefinition: mProject.mStructs) {
    Calculate(structDefinition, structDefinition);
  }

  std::queue<std::string> structsQueue;
  for (const auto &structDefinition: mProject.mStructs) {
    if (mDependencies[structDefinition.mName].empty()) {
      structsQueue.push(structDefinition.mName);
    }
  }

  mProcessOrder.reserve(mProject.mStructs.size());
  std::map<std::string, std::set<std::string>> dependencies = mDependencies;

  while (!structsQueue.empty()) {
    auto &structDefinition = *mProject.GetStruct(structsQueue.front());
    structsQueue.pop();
    mProcessOrder.push_back(structDefinition.mName);
    for (auto &dependentName: mInverseDependencies[structDefinition.mName]) {
      dependencies[dependentName].erase(structDefinition.mName);
      if (dependencies[dependentName].empty()) {
        structsQueue.push(dependentName);
      }
    }
  }
}

void DependencyGraph::Calculate(const StructDefinition &structDefinition,
                                const StructDefinition &curStructDefinition) {
  for (const auto &fieldDefinition: curStructDefinition.mFields) {
    Calculate(structDefinition, fieldDefinition);
  }

  for (const auto &mixin: curStructDefinition.mMixins) {
    Calculate(structDefinition, *mProject.GetStruct(mixin));
  }
}

void DependencyGraph::Calculate(const StructDefinition &structDefinition,
                                const FieldDefinition &fieldDefinition) {
  if (fieldDefinition.mType.mName == "Ref") {
    bool shouldProcess = std::any_of(AnnotationsToCheck.begin(), AnnotationsToCheck.end(),
                                     [&fieldDefinition](const std::string &annotation) {
                                       return fieldDefinition.GetAnnotation(annotation);
                                     });
    if (!shouldProcess)
      return;
  }
  Calculate(structDefinition, fieldDefinition.mType);
}

void DependencyGraph::Calculate(const StructDefinition &structDefinition,
                                const TypeDefinition &typeDefinition) {
  auto referencedStruct = mProject.GetStruct(typeDefinition.mName);
  if (referencedStruct) {
    THROW_IF(mInverseDependencies[structDefinition.mName].contains(referencedStruct->mName),
             "Circular dependency detected between {} and {}", structDefinition.mName,
             referencedStruct->mName)
    THROW_IF(mDependencies[referencedStruct->mName].contains(structDefinition.mName),
             "Circular dependency detected between {} and {}", structDefinition.mName,
             referencedStruct->mName)
    mDependencies[structDefinition.mName].insert(referencedStruct->mName);
    mInverseDependencies[referencedStruct->mName].insert(structDefinition.mName);
  }

  for (const auto &templateParameter: typeDefinition.mTemplateParameters) {
    // This is overly strict - figure out how to handle this better
    if (templateParameter.mName != structDefinition.mName)
      Calculate(structDefinition, templateParameter);
  }
}

const std::vector<std::string> &DependencyGraph::GetProcessOrder() const {
  return mProcessOrder;
}
} // namespace holgen
