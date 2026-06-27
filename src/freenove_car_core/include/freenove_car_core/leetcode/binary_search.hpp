#ifndef FREENOVE_CAR_CORE__LEETCODE__BINARY_SEARCH_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__BINARY_SEARCH_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: The car has a sorted log of timestamps.
/// Find the first reading that exceeds a threshold (time to brake).
/// Binary search on sorted array of sensor timestamps.
///
/// Maps to: LeetCode 704 - Binary Search (classic)
int firstExceeds(const std::vector<int>& sorted_readings, int threshold);

/// Car scenario: The car's battery drains at a known rate per speed.
/// Given speeds, find the minimum speed that drains battery below
/// a target in a given time. (Binary search on answer)
///
/// Maps to: LeetCode 875 - Koko Eating Bananas
int minSpeedToFinish(const std::vector<int>& distances, int time_limit);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__BINARY_SEARCH_HPP_
