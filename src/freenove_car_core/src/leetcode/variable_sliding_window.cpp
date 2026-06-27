#include "freenove_car_core/leetcode/variable_sliding_window.hpp"
#include <unordered_set>
#include <algorithm>
#include <climits>

namespace freenove_car_core {
namespace leetcode {

int longestUniqueSegment(const std::vector<int>& channels) {
  std::unordered_set<int> window;
  int left = 0, max_len = 0;

  for (int right = 0; right < static_cast<int>(channels.size()); ++right) {
    while (window.count(channels[right])) {
      window.erase(channels[left]);
      ++left;
    }
    window.insert(channels[right]);
    max_len = std::max(max_len, right - left + 1);
  }
  return max_len;
}

int minReadingsForThreshold(const std::vector<int>& readings, int threshold) {
  int left = 0, sum = 0, min_len = INT_MAX;

  for (int right = 0; right < static_cast<int>(readings.size()); ++right) {
    sum += readings[right];
    while (sum >= threshold) {
      min_len = std::min(min_len, right - left + 1);
      sum -= readings[left];
      ++left;
    }
  }
  return min_len == INT_MAX ? 0 : min_len;
}

}  // namespace leetcode
}  // namespace freenove_car_core
