#include <cstdio>
#include "leetcode_practice/array_two_sum.hpp"

int main() {
  std::vector<int> nums = {2, 7, 11, 15};
  int target = 9;

  auto result = leetcode_practice::twoSum(nums, target);
  printf("Two Sum: [%d,%d]\n", result[0], result[1]);
  return 0;
}
