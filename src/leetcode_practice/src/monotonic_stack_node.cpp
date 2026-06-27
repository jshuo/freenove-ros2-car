#include <cstdio>
#include "leetcode_practice/monotonic_stack.hpp"

int main() {
  std::vector<int> nums = {2, 1, 2, 4, 3};
  auto result = leetcode_practice::nextGreaterElement(nums);
  printf("Next Greater Elements:\n");
  for (size_t i = 0; i < nums.size(); ++i)
    printf("  %d -> %d\n", nums[i], result[i]);

  std::vector<int> temps = {73, 74, 75, 71, 69, 72, 76, 73};
  auto days = leetcode_practice::dailyTemperatures(temps);
  printf("\nDaily Temperatures:\n");
  for (size_t i = 0; i < temps.size(); ++i)
    printf("  day %zu (%d): wait %d days\n", i, temps[i], days[i]);
  return 0;
}
