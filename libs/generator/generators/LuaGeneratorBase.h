#pragma once
#include "Generator.h"


namespace holgen {
class LuaGeneratorBase : public Generator {
public:
  using Generator::Generator;
protected:
  [[nodiscard]] virtual bool ShouldProcess(const Class &cls) const;
  [[nodiscard]] bool ShouldProcess(const ClassMethod &method) const;
  [[nodiscard]] std::pair<std::string, CodeBlock> ConstructMethodArgumentsAndAnnotations(const ClassMethod &method) const;
  [[nodiscard]] std::string ToLuaType(const Type &type) const;
};

}