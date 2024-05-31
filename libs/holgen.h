#pragma once

#ifdef _DEBUG
#define HOLGEN_DEBUG
#else
#define HOLGEN_RELEASE
#endif

#if defined(WIN32)
#define HOLGEN_WINDOWS
#elif defined(__APPLE__)
#define HOLGEN_OSX
#elif defined(__linux__)
#define HOLGEN_LINUX
#else
#error "Unrecognized platform!"
#endif

#define GEN_GETTER_BY_NAME(cls_name, retval, getterName, field)                                                        \
  const retval *cls_name::getterName(const std::string &name) const {                                                  \
    for (auto &entry: field) {                                                                                         \
      if (entry.mName == name)                                                                                         \
        return &entry;                                                                                                 \
    }                                                                                                                  \
    return nullptr;                                                                                                    \
  }

#define GEN_GETTER_BY_NAME_NONCONST(cls_name, retval, getterName, field)                                               \
  retval *cls_name::getterName(const std::string &name) {                                                              \
    for (auto &entry: field) {                                                                                         \
      if (entry.mName == name)                                                                                         \
        return &entry;                                                                                                 \
    }                                                                                                                  \
    return nullptr;                                                                                                    \
  }

#ifdef HOLGEN_LINUX
#define HOLGEN_UNUSED(param) param __attribute__((unused))
#else
#define HOLGEN_UNUSED(param) param
#endif
