#pragma once

#include <cstdint>


namespace holgen_blackbox_test {
class Race {
public:
  uint32_t GetId() const;
  void SetId(uint32_t val);
  const std::vector<string>& GetNames() const;
  std::vector<string>& GetNames();
  void SetNames(std::vector<string> val);
protected:
private:
  uint32_t mId;
  std::vector<string> mNames;
};
}
