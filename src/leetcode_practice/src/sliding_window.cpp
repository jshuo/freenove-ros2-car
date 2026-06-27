#include "leetcode_practice/sliding_window.hpp"
#include <algorithm>

namespace leetcode_practice {

int maxSubArray(const std::vector<int>& nums) {
  if (nums.empty()) return 0;
  int max_ending_here = nums[0];
  int max_so_far = nums[0];
  for (size_t i = 1; i < nums.size(); ++i) {
    max_ending_here = std::max(nums[i], max_ending_here + nums[i]);
    max_so_far = std::max(max_so_far, max_ending_here);
  }
  return max_so_far;
}

int maxSumSubarray(const std::vector<int>& nums, int k) {
  if (nums.size() < static_cast<size_t>(k) || k <= 0) return 0;
  int window_sum = 0;
  for (int i = 0; i < k; ++i) window_sum += nums[i];
  int max_sum = window_sum;
  for (size_t i = static_cast<size_t>(k); i < nums.size(); ++i) {
    window_sum += nums[i] - nums[i - k];
    max_sum = std::max(max_sum, window_sum);
  }
  return max_sum;
}

}  // namespace leetcode_practice
