#ifndef LARGETASKS_UNORDERED_SET_H
#define LARGETASKS_UNORDERED_SET_H

#include <vector>
#include <list>
#include <iterator>
#include <utility>
#include <algorithm>

template <class KeyT>
class UnorderedSet {
 private:
  std::vector<std::list<KeyT>> table_;
  size_t n_elements_;

  size_t HashFunction(const KeyT& key) const {
    return std::hash<KeyT>{}(key) % table_.size();
  }

 public:
  UnorderedSet() {
    table_ = std::vector<std::list<KeyT>>();
    n_elements_ = 0;
  }

  explicit UnorderedSet(size_t count) {
    n_elements_ = 0;
    table_ = std::vector<std::list<KeyT>>(count);
  }

  template <class IterT>
  UnorderedSet(IterT begin, IterT end) {
    size_t n = std::distance(begin, end);
    n_elements_ = n;
    table_ = std::vector<std::list<KeyT>>(n);
    for (IterT it = begin; it != end; ++it) {
      KeyT value = *it;
      table_[HashFunction(value)].push_back(value);
    }
  }

  UnorderedSet(const UnorderedSet<KeyT>& other_set) {
    table_ = other_set.table_;
    n_elements_ = other_set.n_elements_;
  }

  UnorderedSet<KeyT>& operator=(const UnorderedSet<KeyT>& other_set) {
    if (this != &other_set) {
      table_ = other_set.table_;
      n_elements_ = other_set.n_elements_;
    }
    return *this;
  }

  UnorderedSet(UnorderedSet<KeyT>&& rvalue_other_set) noexcept {
    table_ = std::move(rvalue_other_set.table_);
    n_elements_ = rvalue_other_set.n_elements_;
    rvalue_other_set.n_elements_ = 0;
  }

  UnorderedSet<KeyT>& operator=(UnorderedSet<KeyT>&& rvalue_other_set) noexcept {
    if (this != &rvalue_other_set) {
      table_ = std::move(rvalue_other_set.table_);
      n_elements_ = rvalue_other_set.n_elements_;
      rvalue_other_set.n_elements_ = 0;
    }
    return *this;
  }

  [[nodiscard]] size_t Size() const {
    return n_elements_;
  }

  [[nodiscard]] bool Empty() const {
    return (n_elements_ == 0 || table_.empty());
  }

  void Clear() {
    n_elements_ = 0;
    table_.clear();
  }

  [[nodiscard]] size_t BucketCount() const {
    return table_.size();
  }

  [[nodiscard]] size_t BucketSize(size_t id) const {
    return id >= BucketCount() ? 0 : table_[id].size();
  }

  size_t Bucket(const KeyT& key) const {
    return HashFunction(key);
  }

  [[nodiscard]] double LoadFactor() const {
    return static_cast<double>(n_elements_) / (table_.empty() ? 1 : static_cast<double>(table_.size()));
  }

  void Rehash(size_t new_bucket_count) {
    if (new_bucket_count >= n_elements_) {
      std::vector<std::list<KeyT>> tmp_table(new_bucket_count);
      for (const std::list<KeyT>& cur_bucket : table_) {
        for (const KeyT& cur_value : cur_bucket) {
          size_t new_hash = std::hash<KeyT>{}(cur_value) % new_bucket_count;
          tmp_table[new_hash].push_back(cur_value);
        }
      }
      std::swap(table_, tmp_table);
    }
  }

  void Reserve(size_t new_bucket_count) {
    if (new_bucket_count > table_.size()) {
      Rehash(new_bucket_count);
    }
  }

  void Erase(const KeyT& value) {
    size_t value_hash = HashFunction(value);
    auto value_bucket_iter = std::find(table_[value_hash].begin(), table_[value_hash].end(), value);
    if (value_bucket_iter != table_[value_hash].end()) {
      table_[value_hash].erase(value_bucket_iter);
      n_elements_--;
    }
  }

  bool Find(const KeyT& value) const {
    if (BucketCount() == 0) {
      return false;
    }
    size_t value_hash = HashFunction(value);
    return (std::find(table_[value_hash].begin(), table_[value_hash].end(), value) != table_[value_hash].end());
  }

  void Insert(const KeyT& value) {
    if (!Find(value)) {
      if (BucketCount() == 0) {
        table_ = std::vector<std::list<KeyT>>(1);
      }
      if (LoadFactor() >= 1.0) {
        Reserve(n_elements_ * 2);
      }
      table_[HashFunction(value)].push_back(value);
      n_elements_++;
    }
  }

  void Insert(KeyT&& r_value) {
    Insert(std::as_const(r_value));
  }

  ~UnorderedSet() = default;
};

#endif
