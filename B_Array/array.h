#define ARRAY_TRAITS_IMPLEMENTED
#ifndef LARGETASKS_ARRAY_H
#define LARGETASKS_ARRAY_H

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <algorithm>

class ArrayOutOfRange : std::exception {};

template <class T, size_t N>
class Array {
 public:
  T inner_array_[N];

  T& operator[](size_t n) {
    return inner_array_[n];
  }

  const T& operator[](size_t n) const {
    return inner_array_[n];
  }

  T& At(size_t n) {
    if (n >= N) {
      throw ArrayOutOfRange{};
    }
    return inner_array_[n];
  }

  const T& At(size_t n) const {
    if (n >= N) {
      throw ArrayOutOfRange{};
    }
    return inner_array_[n];
  }

  T& Front() {
    return inner_array_[0];
  }

  const T& Front() const {
    return inner_array_[0];
  }

  T& Back() {
    return inner_array_[N - 1];
  }

  const T& Back() const {
    return inner_array_[N - 1];
  }

  T* Data() {
    return inner_array_;
  }

  const T* Data() const {
    return inner_array_;
  }

  [[nodiscard]] size_t Size() const {
    return N;
  }

  [[nodiscard]] bool Empty() const {
    return N == 0;
  }

  void Fill(const T& value) {
    for (size_t i = 0u; i < N; ++i) {
      inner_array_[i] = value;
    }
  }

  void Swap(Array<T, N>& other) {
    std::swap(inner_array_, other.inner_array_);
  }
};

template <class T>
size_t GetSize(const T&) {
  return 0;
}

template <class T, size_t N>
size_t GetSize(const T (&)[N]) {
  return N;
}

template <class T>
size_t GetRank(const T&) {
  return 0;
}

template <class T, size_t N>
size_t GetRank(const T (&)[N]) {
  return GetRank(T{}) + 1;
}

template <class T>
size_t GetNumElements(const T&) {
  return 1;
}

template <class T, size_t N>
size_t GetNumElements(const T (&)[N]) {
  return GetNumElements(T{}) * N;
}

#endif
