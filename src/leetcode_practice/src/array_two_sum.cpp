#include "leetcode_practice/array_two_sum.hpp"
#include <unordered_map>

namespace leetcode_practice {

std::vector<int> twoSum(const std::vector<int>& nums, int target) {
  std::unordered_map<int, int> seen;
  for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
    int complement = target - nums[i];
    auto it = seen.find(complement);
    if (it != seen.end()) {
      return {it->second, i};
    }
    seen[nums[i]] = i;
  }
  return {};
}

}  // namespace leetcode_practice
