#ifndef FREENOVE_CAR_CORE__LEETCODE__PREFIX_SUM_HASHING_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__PREFIX_SUM_HASHING_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: The car logs voltage readings over time.
/// Count how many continuous segments have total voltage == target.
/// (Useful for detecting specific patterns in sensor data)
///
/// Maps to: LeetCode 560 - Subarray Sum Equals K
/// (Prefix sum + hash map — HIGH Amazon frequency)
int countSegmentsWithSum(const std::vector<int>& readings, int target);

/// Car scenario: Find the longest continuous segment where the sum
/// of sensor values == target (Maximum length variant)
///
/// Maps to: LeetCode 325 - Maximum Size Subarray Sum Equals K
int longestSegmentWithSum(const std::vector<int>& readings, int target);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__PREFIX_SUM_HASHING_HPP_
