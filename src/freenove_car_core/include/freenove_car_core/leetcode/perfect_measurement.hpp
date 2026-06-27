#ifndef FREENOVE_CAR_CORE__LEETCODE__PERFECT_MEASUREMENT_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__PERFECT_MEASUREMENT_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: You have a set of sensor readings. Find if any 3 readings
/// (at different timestamps) sum exactly to a target value.
///
/// Maps to: LeetCode 15 - 3Sum (with Two Pointers)
///
/// Input:  readings = {12, 3, 5, 8, 1, 6, 4}, target = 12
/// Output: true  (e.g., 3 + 5 + 4 = 12)
bool hasPerfectMeasurement(const std::vector<int>& readings, int target);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__PERFECT_MEASUREMENT_HPP_
