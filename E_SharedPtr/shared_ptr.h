#define WEAK_PTR_IMPLEMENTED
#ifndef LARGETASKS_SHARED_PTR_H
#define LARGETASKS_SHARED_PTR_H
#include <cstdint>
#include <algorithm>
#include <exception>

class BadWeakPtr : std::exception {};

template <class T>
class WeakPtr;

struct Counter {
  int32_t strong_counter_;
  int32_t weak_counter_;

  void AddStrongCounter() {
    strong_counter_++;
  }
  void RemoveStrongCounter() {
    strong_counter_--;
  }
  void AddWeakCounter() {
    weak_counter_++;
  }
  void RemoveWeakCounter() {
    weak_counter_--;
  }
};

template <class T>
class SharedPtr {
 private:
  T* pointer_;
  Counter* refs_counter_;

 public:
  explicit SharedPtr(const WeakPtr<T>& weak_ptr);

  SharedPtr() noexcept {
    pointer_ = nullptr;
    refs_counter_ = nullptr;
  }

  explicit SharedPtr(T* pointer) {
    pointer_ = pointer;
    refs_counter_ = (pointer_ == nullptr ? nullptr : new Counter{1, 0});
  }

  SharedPtr(const SharedPtr<T>& other_ptr) {
    pointer_ = other_ptr.pointer_;
    refs_counter_ = other_ptr.refs_counter_;
    if (refs_counter_) {
      refs_counter_->AddStrongCounter();
    }
  }

  SharedPtr<T>& operator=(const SharedPtr& other_ptr) {
    if (this != &other_ptr) {
      CleanStrongPointer();
      pointer_ = other_ptr.pointer_;
      refs_counter_ = other_ptr.refs_counter_;
      if (refs_counter_) {
        refs_counter_->AddStrongCounter();
      }
    }
    return *this;
  }

  SharedPtr(SharedPtr<T>&& rvalue_ptr) noexcept {
    pointer_ = rvalue_ptr.pointer_;
    refs_counter_ = rvalue_ptr.refs_counter_;
    rvalue_ptr.pointer_ = nullptr;
    rvalue_ptr.refs_counter_ = nullptr;
  }

  SharedPtr<T>& operator=(SharedPtr<T>&& rvalue_ptr) noexcept {
    if (this != &rvalue_ptr) {
      CleanStrongPointer();
      pointer_ = rvalue_ptr.pointer_;
      refs_counter_ = rvalue_ptr.refs_counter_;
      rvalue_ptr.pointer_ = nullptr;
      rvalue_ptr.refs_counter_ = nullptr;
    }
    return *this;
  }

  void Reset(T* ptr = nullptr) {
    if (ptr != pointer_) {
      CleanStrongPointer();
      pointer_ = ptr;
      refs_counter_ = (ptr == nullptr ? nullptr : new Counter{1, 0});
    }
  }

  [[nodiscard]] int32_t UseCount() const {  // NOLINT
    return ((refs_counter_ == nullptr) ? 0 : refs_counter_->strong_counter_);
  }

  void Swap(SharedPtr<T>& ptr) {
    std::swap(pointer_, ptr.pointer_);
    std::swap(refs_counter_, ptr.refs_counter_);
  }

  explicit operator bool() const {
    return pointer_ != nullptr;
  }

  T& operator*() const noexcept {
    return *pointer_;
  }

  T* Get() const noexcept {
    return pointer_;
  }

  [[nodiscard]] Counter* GetCounter() const noexcept {
    return refs_counter_;
  }

  T* operator->() const noexcept {
    return pointer_;
  }

  void CleanStrongPointer() noexcept {
    if (refs_counter_ != nullptr) {
      refs_counter_->RemoveStrongCounter();
      if (refs_counter_->strong_counter_ <= 0 && refs_counter_->weak_counter_ <= 0) {
        delete pointer_;
        delete refs_counter_;
      } else if (refs_counter_->strong_counter_ <= 0 && refs_counter_->weak_counter_ > 0) {
        delete pointer_;
      }
    }
    pointer_ = nullptr;
  }

  ~SharedPtr() {
    CleanStrongPointer();
  }
};

template <class T>
class WeakPtr {
 private:
  T* pointer_;
  Counter* refs_counter_;

 public:
  WeakPtr() noexcept {
    pointer_ = nullptr;
    refs_counter_ = nullptr;
  }

  WeakPtr(const WeakPtr<T>& other_ptr) noexcept {
    pointer_ = other_ptr.pointer_;
    refs_counter_ = other_ptr.refs_counter_;
    if (refs_counter_) {
      refs_counter_->AddWeakCounter();
    }
  }

  WeakPtr<T>& operator=(const WeakPtr& other_ptr) noexcept {
    if (this != &other_ptr) {
      CleanWeakPointer();
      pointer_ = other_ptr.pointer_;
      refs_counter_ = other_ptr.refs_counter_;
      if (refs_counter_) {
        refs_counter_->AddWeakCounter();
      }
    }
    return *this;
  }

  WeakPtr(WeakPtr<T>&& rvalue_ptr) noexcept {
    pointer_ = rvalue_ptr.pointer_;
    refs_counter_ = rvalue_ptr.refs_counter_;
    rvalue_ptr.pointer_ = nullptr;
    rvalue_ptr.refs_counter_ = nullptr;
  }

  WeakPtr<T>& operator=(WeakPtr<T>&& rvalue_ptr) noexcept {
    if (this != &rvalue_ptr) {
      CleanWeakPointer();
      pointer_ = rvalue_ptr.pointer_;
      refs_counter_ = rvalue_ptr.refs_counter_;
      rvalue_ptr.pointer_ = nullptr;
      rvalue_ptr.refs_counter_ = nullptr;
    }
    return *this;
  }

  WeakPtr(const SharedPtr<T>& shared_ptr) {  // NOLINT
    pointer_ = shared_ptr.Get();
    refs_counter_ = shared_ptr.GetCounter();
    if (refs_counter_) {
      refs_counter_->AddWeakCounter();
    }
  }

  void Swap(WeakPtr& other_weak_ptr) {
    std::swap(pointer_, other_weak_ptr.pointer_);
    std::swap(refs_counter_, other_weak_ptr.refs_counter_);
  }

  void Reset() noexcept {
    CleanWeakPointer();
    pointer_ = nullptr;
  }

  [[nodiscard]] int32_t UseCount() const {  // NOLINT
    return ((refs_counter_ == nullptr) ? 0 : refs_counter_->strong_counter_);
  }

  [[nodiscard]] bool Expired() const noexcept {
    return (refs_counter_ == nullptr || refs_counter_->strong_counter_ <= 0);
  }

  T* Get() const noexcept {
    return pointer_;
  }

  [[nodiscard]] Counter* GetCounter() const noexcept {
    return refs_counter_;
  }

  SharedPtr<T> Lock() const noexcept {
    return (Expired() ? SharedPtr<T>() : SharedPtr<T>(*this));
  }

  void CleanWeakPointer() noexcept {
    if (refs_counter_) {
      refs_counter_->RemoveWeakCounter();
      if (refs_counter_->strong_counter_ <= 0 && refs_counter_->weak_counter_ <= 0) {
        delete refs_counter_;
      }
      refs_counter_ = nullptr;
    }
  }

  ~WeakPtr() {
    CleanWeakPointer();
  }
};

template <class T>
SharedPtr<T>::SharedPtr(const WeakPtr<T>& weak_ptr) {
  if (weak_ptr.Expired()) {
    throw BadWeakPtr{};
  }
  pointer_ = weak_ptr.Get();
  refs_counter_ = weak_ptr.GetCounter();
  if (refs_counter_) {
    refs_counter_->AddStrongCounter();
  }
}

template <class T, class... Args>
SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
};

#endif
