#ifndef LARGETASKS_VECTOR_H
#define LARGETASKS_VECTOR_H

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <utility>
#include <memory>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

template <class T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  //  using Iterator = decltype(std::begin(std::declval<T&>()));
  //  using ConstIterator = decltype(std::begin(std::declval<const T&>()));

 private:
  T* inner_array_;
  size_t n_elements_ = 0;
  size_t capacity_ = 0;

  void DeallocateVectorMemory() noexcept {
    //    std::destroy(begin(inner_array_), end(inner_array_));
    //    for (size_t i = 0u; i < n_elements_; ++i) {
    //      DeallocateTopValue();
    //    }
    n_elements_ = 0;
    capacity_ = 0;
    delete[] inner_array_;
  }

  void DeallocateTopValue() noexcept {
    try {
      if (n_elements_ != 0) {
        std::destroy_at((inner_array_ + n_elements_));
        n_elements_--;
      }
    } catch (...) {
    }
  }

  void AllocateNewValue(T& value_to_place) noexcept {
    T* placement_mem = nullptr;
    try {
      if (!std::is_copy_constructible<T>().value) {
        placement_mem = new ((inner_array_ + n_elements_)) T(std::forward<T>(value_to_place));
      } else {
        placement_mem = new ((inner_array_ + n_elements_)) T(value_to_place);
      }
      n_elements_++;
    } catch (...) {
      DeallocateTopValue();
      delete placement_mem;
    }
  }

 public:
  Vector() noexcept {
    try {
      inner_array_ = new T[0];
      n_elements_ = 0;
      capacity_ = 0;
    } catch (...) {
      DeallocateVectorMemory();
      throw;
    }
  }

  explicit Vector(size_t n) noexcept {
    try {
      inner_array_ = new T[n];
      capacity_ = n;
      n_elements_ = n;
    } catch (...) {
      DeallocateVectorMemory();
      throw;
    }
  }

  Vector(size_t n, T value) noexcept {
    try {
      inner_array_ = new T[n];
      for (size_t i = 0u; i < n; ++i) {
        AllocateNewValue(value);
      }
      capacity_ = n;
    } catch (...) {
      DeallocateVectorMemory();
      throw;
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator begin, Iterator end) noexcept {
    try {
      size_t n = std::distance(begin, end);
      inner_array_ = new T[n];
      capacity_ = n;
      for (Iterator it = begin; it != end; ++it) {
        T value = *it;
        AllocateNewValue(value);
      }
    } catch (...) {
      DeallocateVectorMemory();
      throw;
    }
  }

  Vector(std::initializer_list<T> init_list) noexcept {
    Vector(init_list.begin(), init_list.end());
  }

  Vector(const Vector<T>& other_vector) noexcept {
    this->inner_array_ = other_vector.inner_array_;
    this->capacity_ = other_vector.capacity_;
    this->n_elements_ = other_vector.n_elements_;
  }

  Vector<T>& operator=(const Vector<T>& other_vector) noexcept {
    if (this != &other_vector) {
      DeallocateVectorMemory();
      this->inner_array_ = other_vector.inner_array_;
      this->capacity_ = other_vector.capacity_;
      this->n_elements_ = other_vector.n_elements_;
    }
    return *this;
  }

  Vector(Vector<T>&& rvalue_vector) noexcept {
    this->inner_array_ = std::move(rvalue_vector.inner_array_);
    this->n_elements_ = rvalue_vector.n_elements_;
    this->capacity_ = rvalue_vector.capacity_;
    rvalue_vector.capacity_ = 0;
    rvalue_vector.n_elements_ = 0;
  }

  Vector<T>& operator=(Vector<T>&& rvalue_vector) noexcept {
    if (this != &rvalue_vector) {
      DeallocateVectorMemory();
      this->inner_array_ = std::move(rvalue_vector.inner_array_);
      this->n_elements_ = rvalue_vector.n_elements_;
      this->capacity_ = rvalue_vector.capacity_;
      rvalue_vector.capacity_ = 0;
      rvalue_vector.n_elements_ = 0;
    }
    return *this;
  }

  [[nodiscard]] size_t Size() const noexcept {
    return n_elements_;
  }

  [[nodiscard]] size_t Capacity() const noexcept {
    return capacity_;
  }

  [[nodiscard]] bool Empty() const noexcept {
    return (n_elements_ == 0);
  }

  const T& operator[](size_t n) const {
    return inner_array_[n];
  }

  T& operator[](size_t n) {
    return inner_array_[n];
  }

  const T& At(size_t n) const noexcept {
    if (n >= n_elements_) {
      throw std::out_of_range("");
    }
    return inner_array_[n];
  }

  T& At(size_t n) noexcept {
    if (n >= n_elements_) {
      throw std::out_of_range("");
    }
    return inner_array_[n];
  }

  const T& Back() const noexcept {
    return inner_array_[n_elements_ - 1];
  }

  T& Back() noexcept {
    return inner_array_[n_elements_ - 1];
  }

  const T& Front() const noexcept {
    return inner_array_[0];
  }

  T& Front() noexcept {
    return inner_array_[0];
  }

  const T* Data() const noexcept {
    return (n_elements_ == 0 ? nullptr : inner_array_);
  }

  T* Data() noexcept {
    return (n_elements_ == 0 ? nullptr : inner_array_);
  }

  void Swap(Vector<T>& other) {
    std::swap(inner_array_, other.inner_array_);
    std::swap(capacity_, other.capacity_);
    std::swap(n_elements_, other.n_elements_);
  }

  ~Vector() noexcept {
    DeallocateVectorMemory();
  }
};

#endif
