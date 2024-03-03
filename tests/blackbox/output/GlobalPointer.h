#pragma once

namespace holgen_blackbox_test {
template <typename T>
class GlobalPointer {
public:
  static T* GetInstance() {
    return mInstance;
  }
  static void SetInstance(T* ptr) {
    mInstance = ptr;
  }
protected:
private:
  inline static T* mInstance = nullptr;
};
}
