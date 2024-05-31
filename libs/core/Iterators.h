#pragma once

#include <string>

namespace holgen {
template <class T>
struct NameFilterIterator {
  NameFilterIterator(std::string name, T begin, T end) : mName(std::move(name)), mIterator(begin), mEnd(end) { Next(); }

  auto &operator*() { return *mIterator; }

  auto operator++() {
    ++mIterator;
    Next();
    return *this;
  }

  bool operator!=(const NameFilterIterator &other) const { return mIterator != other.mIterator; }

private:
  std::string mName;
  T mIterator;
  T mEnd;

  void Next() {
    while (mIterator != mEnd && mIterator->mName != mName) {
      ++mIterator;
    }
  }
};

template <class T>
struct NameFilterForEachWrapper {
  NameFilterForEachWrapper(std::string name, T &container) : mName(std::move(name)), mContainer(container) {}

  auto begin() { return NameFilterIterator(mName, mContainer.begin(), mContainer.end()); }

  auto end() { return NameFilterIterator("", mContainer.end(), mContainer.end()); }

  std::string mName;
  const T &mContainer;
};
} // namespace holgen
