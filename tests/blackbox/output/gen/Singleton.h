// This file is generated by holgen. Do not modify manually.
#pragma once

#include "../holgen.h"
namespace holgen_blackbox_test {
template <typename T>
class Singleton {
public:
  static T *GetSingleton() {
    return mInstance;
  }
  static T &GetInstance() {
    return *mInstance;
  }
  static void SetSingleton(T *val) {
    HOLGEN_FAIL_IF(mInstance, "SetSingleton called on a class that already has an instance!");
    mInstance = val;
  }
  static void DeleteSingleton() {
    HOLGEN_FAIL_IF(!mInstance, "DeleteSingleton called on a class that does not have an instance!");
    delete mInstance;
    mInstance = nullptr;
  }
private:
  inline static T *mInstance = nullptr;
};
}
