#ifndef FREENOVE_CAR_CORE__LEETCODE__SENSOR_PATTERN_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__SENSOR_PATTERN_HPP_

#include <vector>
#include <utility>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: Onboard firmware checks a rolling window of telemetry.
/// If two sensor readings sum exactly to a known fault threshold,
/// flag a potential issue before it escalates.
///
/// Readings arrive as pairs: (sensor_A_value, sensor_B_value).
/// Find if any two readings (from different ticks) sum to target.
///
/// Maps to: LeetCode 1 - Two Sum
///
/// Input:  {(12, 8), (3, 17), (5, 5), ...}
/// Target: 20
/// Output: (0, 1)  — tick 0 (12+8) + tick 1 (3+17) = 20+20=40 ... 
/// Actually: Two Sum on telemetry values {20, 20, 10, ...} target=40
/// Returns indices in the flattened value array, or {-1,-1}.
std::pair<int, int> findFaultPattern(const std::vector<int>& telemetry,
                                     int fault_threshold);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__SENSOR_PATTERN_HPP_
