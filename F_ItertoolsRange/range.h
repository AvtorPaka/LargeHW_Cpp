#define REVERSE_RANGE_IMPLEMENTED
#ifndef LARGETASKS_ITERTOOLSRANGE_RANGE_H
#define LARGETASKS_ITERTOOLSRANGE_RANGE_H

#include <cstdint>

class Iterator {
 public:
  int32_t step_;
  int32_t inner_number_;
  Iterator(int32_t num, int32_t step) {
    step_ = step;
    inner_number_ = num;
  }
  Iterator() : Iterator(0, 1) {
  }
  explicit Iterator(int32_t num) : Iterator(num, 1) {
  }

  int32_t& operator*() noexcept {
    return inner_number_;
  }

  const int32_t& operator*() const noexcept {
    return inner_number_;
  }

  int32_t& operator++() {
    inner_number_ += step_;
    return inner_number_;
  }

  int32_t operator++(int) {
    int32_t old_value = inner_number_;
    inner_number_ += step_;
    return old_value;
  }

  bool operator!=(const Iterator& other) const {
    if (step_ > 0) {
      return (inner_number_ < *other);
    }
    return (inner_number_ > *other);
  }

  ~Iterator() = default;
};

class IteratorRange {
 private:
  Iterator begin_iter_;
  Iterator end_iter_;

 public:
  IteratorRange(int32_t num_begin, int32_t num_end, int32_t step) {
    begin_iter_ = Iterator(num_begin, step);
    end_iter_ = Iterator(num_end, step);
  }

  IteratorRange(int32_t num_begin, int32_t num_end) {
    begin_iter_ = Iterator(num_begin);
    end_iter_ = Iterator(num_end);
  }

  explicit IteratorRange(int32_t num_end) {
    begin_iter_ = Iterator();
    end_iter_ = Iterator(num_end);
  }

  IteratorRange() : IteratorRange(0) {
  }

  Iterator& begin() {  // NOLINT
    return begin_iter_;
  }

  Iterator& end() {  // NOLINT
    return end_iter_;
  }

  [[nodiscard]] const Iterator& begin() const {  // NOLINT
    return begin_iter_;
  }

  [[nodiscard]] const Iterator& end() const {  // NOLINT
    return end_iter_;
  }

  Iterator rbegin() {  // NOLINT
    int32_t new_begin = 0;
    if ((end_iter_.inner_number_ - begin_iter_.inner_number_) % end_iter_.step_ != 0) {
      if (end_iter_.step_ > 0) {
        new_begin = end_iter_.inner_number_ - 1;
      } else {
        new_begin = end_iter_.inner_number_ + 2;
      }
    } else {
      new_begin = end_iter_.inner_number_ - end_iter_.step_;
    }
    int32_t new_step = -end_iter_.step_;
    return {new_begin, new_step};
  }

  const Iterator rbegin() const {  // NOLINT
    int32_t new_begin = 0;
    if ((end_iter_.inner_number_ - begin_iter_.inner_number_) % end_iter_.step_ != 0) {
      if (end_iter_.step_ > 0) {
        new_begin = end_iter_.inner_number_ - 1;
      } else {
        new_begin = end_iter_.inner_number_ + 2;
      }
    } else {
      new_begin = end_iter_.inner_number_ - end_iter_.step_;
    }
    int32_t new_step = -end_iter_.step_;
    return {new_begin, new_step};
  }

  Iterator rend() {  // NOLINT
    int32_t new_end = begin_iter_.inner_number_ - begin_iter_.step_;
    int32_t new_step = -begin_iter_.step_;
    return {new_end, new_step};
  }

  const Iterator rend() const {  // NOLINT
    int32_t new_end = begin_iter_.inner_number_ - begin_iter_.step_;
    int32_t new_step = -begin_iter_.step_;
    return {new_end, new_step};
  }
};

inline IteratorRange Range(int32_t end_num) {
  if (end_num <= 0) {
    return {};
  }
  return IteratorRange(end_num);
}

inline IteratorRange Range(int32_t begin_num, int32_t end_num) {
  if (end_num <= begin_num) {
    return {};
  }
  return {begin_num, end_num};
}

inline IteratorRange Range(int32_t begin_num, int32_t end_num, int32_t step) {
  if (step == 0 || (step > 0 && end_num <= begin_num) || (step < 0 && end_num >= begin_num)) {
    return {};
  }
  return {begin_num, end_num, step};
}

#endif
