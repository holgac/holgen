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
    std::set<std::string> processedMixins{structDefinition.mName};
    Calculate(structDefinition, structDefinition, processedMixins);
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
                                const StructDefinition &curStructDefinition,
                                std::set<std::string> &processedMixins) {
  for (const auto &fieldDefinition: curStructDefinition.mFields) {
    Calculate(structDefinition, fieldDefinition);
  }

  for (const auto &mixin: curStructDefinition.mMixins) {
    auto mixinDefinition = mProject.GetStruct(mixin);
    THROW_IF(!mixinDefinition, "{} ({}) uses unrecognized mixin {}", curStructDefinition.mName,
             curStructDefinition.mDefinitionSource, mixin);
    THROW_IF(processedMixins.contains(mixin),
             "Circular or duplicate mixin usage detected in {} ({}) and {} ({})",
             mixinDefinition->mName, mixinDefinition->mDefinitionSource, curStructDefinition.mName,
             curStructDefinition.mDefinitionSource);
    processedMixins.insert(mixin);
    Calculate(structDefinition, *mixinDefinition, processedMixins);
  }
}

void DependencyGraph::Calculate(const StructDefinition &structDefinition,
                                const FieldDefinition &fieldDefinition) {
  Calculate(structDefinition, fieldDefinition, fieldDefinition.mType);
}

void DependencyGraph::Calculate(const StructDefinition &structDefinition,
                                const FieldDefinition &fieldDefinition,
                                const TypeDefinition &typeDefinition) {
  if (typeDefinition.mName == "Ref") {
    bool shouldProcess = std::any_of(AnnotationsToCheck.begin(), AnnotationsToCheck.end(),
                                     [&fieldDefinition](const std::string &annotation) {
                                       return fieldDefinition.GetAnnotation(annotation);
                                     });
    if (!shouldProcess)
      return;
  }
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
      Calculate(structDefinition, fieldDefinition, templateParameter);
  }
}

const std::vector<std::string> &DependencyGraph::GetProcessOrder() const {
  return mProcessOrder;
}
} // namespace holgen
