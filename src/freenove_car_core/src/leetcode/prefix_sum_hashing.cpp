#include "freenove_car_core/leetcode/prefix_sum_hashing.hpp"
#include <unordered_map>
#include <algorithm>

namespace freenove_car_core {
namespace leetcode {

int countSegmentsWithSum(const std::vector<int>& readings, int target) {
  std::unordered_map<int, int> prefix_count;
  prefix_count[0] = 1;
  int sum = 0, count = 0;

  for (int val : readings) {
    sum += val;
    count += prefix_count[sum - target];
    ++prefix_count[sum];
  }
  return count;
}

int longestSegmentWithSum(const std::vector<int>& readings, int target) {
  std::unordered_map<int, int> first_occurrence;
  first_occurrence[0] = -1;
  int sum = 0, max_len = 0;

  for (int i = 0; i < static_cast<int>(readings.size()); ++i) {
    sum += readings[i];
    if (first_occurrence.count(sum - target)) {
      max_len = std::max(max_len, i - first_occurrence[sum - target]);
    }
    if (!first_occurrence.count(sum)) {
      first_occurrence[sum] = i;
    }
  }
  return max_len;
}

}  // namespace leetcode
}  // namespace freenove_car_core
