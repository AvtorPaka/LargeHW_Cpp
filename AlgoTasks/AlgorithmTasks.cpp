#include "AlgorithmTasks.h"

#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <queue>

struct PairComparer {
  bool operator()(const std::pair<int32_t, std::string>& pair_1, const std::pair<int32_t, std::string>& pair_2) {
    if (pair_1.first == pair_2.first) {
      return pair_1.second < pair_2.second;
    }
    return pair_1.first > pair_2.first;
  }
};

void AlgorithmTasks::FrequencyAnalysis() {
  std::unordered_map<std::string, int32_t> word_map;
  std::string line;
  while (std::cin >> line) {
    word_map[line]++;
  }

  size_t n = word_map.size();
  std::vector<std::pair<int32_t, std::string>> words_array;
  words_array.reserve(n);

  for (auto const& [word, num] : word_map) {
    words_array.emplace_back(num, word);
  }
  std::sort(words_array.begin(), words_array.end(), PairComparer{});

  for (uint32_t i = 0; i < n; ++i) {
    std::cout << words_array[i].second << '\n';
  }
}

void AlgorithmTasks::WordRate() {
  std::unordered_map<std::string, int32_t> words_map;

  std::string cur_word;
  while (std::cin >> cur_word) {
    if (!cur_word.empty()) {
      std::cout << words_map[cur_word]++ << " ";
    }
  }
}
void AlgorithmTasks::BigPolitics() {
  int32_t n = 0;
  std::cin >> n;
  std::priority_queue<int32_t, std::vector<int32_t>, std::greater<>> num_queue;

  for (int32_t i = 0; i < n; ++i) {
    int32_t cur_num = 0;
    std::cin >> cur_num;
    num_queue.emplace(cur_num);
  }

  int32_t sum_ans = 0;
  while (num_queue.size() != 1) {
    int32_t first_num = num_queue.top();
    num_queue.pop();
    int32_t second_num = num_queue.top();
    num_queue.pop();
    int32_t cur_sum = first_num + second_num;
    num_queue.emplace(cur_sum);
    sum_ans += cur_sum;
  }

  std::cout << sum_ans << std::endl;
}

void AlgorithmTasks::Passwords() {
  std::unordered_map<std::string, int32_t> nums_of_substring;
  int32_t ans = 0;

  int32_t n = 0;
  std::cin >> n;
  std::vector<std::string> passwords(n);
  passwords.reserve(n);
  for (int32_t i = 0; i < n; ++i) {
    std::cin >> passwords[i];
  }
  std::sort(passwords.begin(), passwords.end(),
            [](std::string& lhs, std::string& rhs) -> bool { return lhs.size() < rhs.size(); });

  for (int32_t i = 0; i < n; ++i) {
    std::string cur_word = passwords[i];
    if (nums_of_substring.find(cur_word) == nums_of_substring.end()) {
      for (int32_t j = 0; j < n; ++j) {
        if (i == j) {
          continue;
        }
        if (passwords[j].find(cur_word) != std::string::npos) {
          nums_of_substring[cur_word]++;
        }
      }
    }
    ans += nums_of_substring[cur_word];
  }

  std::cout << ans;
}
