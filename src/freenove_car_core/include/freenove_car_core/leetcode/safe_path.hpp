#ifndef FREENOVE_CAR_CORE__LEETCODE__SAFE_PATH_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__SAFE_PATH_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: You're driving and reading ultrasonic distances.
/// Find the longest contiguous segment where ALL distances are >= min_safe_cm.
///
/// Maps to: LeetCode 3 - Longest Substring Without Repeating Characters
/// (Variable-length sliding window with a condition)
///
/// Input:  sensor_readings = [50, 60, 15, 80, 90, 100, 5, 70], min_safe = 20
/// Output: 3  (indices 3-5: 80, 90, 100)
int longestSafePath(const std::vector<int>& sensor_readings, int min_safe_cm);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__SAFE_PATH_HPP_
