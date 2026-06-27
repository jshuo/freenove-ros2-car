#ifndef LEETCODE_PRACTICE__MONOTONIC_STACK_HPP_
#define LEETCODE_PRACTICE__MONOTONIC_STACK_HPP_

#include <vector>

namespace leetcode_practice {

/// LeetCode 496: Next Greater Element I
/// For each element in nums, find the next greater element to its right.
/// Returns -1 if no such element exists.
std::vector<int> nextGreaterElement(const std::vector<int>& nums);

/// LeetCode 739: Daily Temperatures
/// Given temperatures, return an array days[i] = number of days until
/// a warmer temperature. 0 if no warmer day ahead.
std::vector<int> dailyTemperatures(const std::vector<int>& temperatures);

}  // namespace leetcode_practice

#endif  // LEETCODE_PRACTICE__MONOTONIC_STACK_HPP_
