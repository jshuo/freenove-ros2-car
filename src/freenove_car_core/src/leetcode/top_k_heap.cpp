#include "freenove_car_core/leetcode/top_k_heap.hpp"
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <vector>

namespace freenove_car_core {
namespace leetcode {

std::vector<int> topKFrequentReadings(const std::vector<int>& readings, int k) {
  // Count frequencies
  std::unordered_map<int, int> freq;
  for (int v : readings) ++freq[v];

  // Min-heap of (frequency, value), keep top k
  using Pair = std::pair<int, int>;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> min_heap;

  for (const auto& [val, count] : freq) {
    min_heap.push({count, val});
    if (static_cast<int>(min_heap.size()) > k) {
      min_heap.pop();
    }
  }

  std::vector<int> result(k);
  for (int i = k - 1; i >= 0; --i) {
    result[i] = min_heap.top().second;
    min_heap.pop();
  }
  return result;
}

std::vector<int> mergeSortedLogs(const std::vector<std::vector<int>>& logs) {
  using Entry = std::pair<int, std::pair<int, int>>;  // (value, (log_idx, pos_idx))
  std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> min_heap;

  // Push first element from each non-empty log
  for (int i = 0; i < static_cast<int>(logs.size()); ++i) {
    if (!logs[i].empty()) {
      min_heap.push({logs[i][0], {i, 0}});
    }
  }

  std::vector<int> merged;
  while (!min_heap.empty()) {
    auto [val, idx] = min_heap.top(); min_heap.pop();
    merged.push_back(val);
    int log_i = idx.first, pos = idx.second + 1;
    if (pos < static_cast<int>(logs[log_i].size())) {
      min_heap.push({logs[log_i][pos], {log_i, pos}});
    }
  }
  return merged;
}

}  // namespace leetcode
}  // namespace freenove_car_core
