#include <cstdio>
#include "leetcode_practice/sliding_window.hpp"

int main() {
  std::vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  int ms = leetcode_practice::maxSubArray(nums);
  printf("Max Subarray: %d\n", ms);

  std::vector<int> nums2 = {1, 3, -1, -3, 5, 3, 6, 7};
  int k = 3;
  int mss = leetcode_practice::maxSumSubarray(nums2, k);
  printf("Max Sum Subarray (k=%d): %d\n", k, mss);
  return 0;
}
