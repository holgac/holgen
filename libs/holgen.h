#pragma once

#define GEN_GETTER_BY_NAME(cls_name, retval, getterName, field) \
  const retval *cls_name::getterName(const std::string &name) const { \
    for (auto &entry: field) { \
      if (entry.mName == name) \
        return &entry; \
    } \
    return nullptr; \
}

#define GEN_GETTER_BY_NAME_NONCONST(cls_name, retval, getterName, field) \
  retval *cls_name::getterName(const std::string &name) { \
    for (auto &entry: field) { \
      if (entry.mName == name) \
        return &entry; \
    } \
  return nullptr; \
  }
