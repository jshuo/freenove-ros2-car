#ifndef FREENOVE_CAR_CORE__LEETCODE__VARIABLE_SLIDING_WINDOW_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__VARIABLE_SLIDING_WINDOW_HPP_

#include <string>
#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: The car receives a stream of sensor channel IDs.
/// Find the longest segment without any repeated channel ID.
/// (e.g., the camera should not see the same landmark twice in a row)
///
/// Maps to: LeetCode 3 - Longest Substring Without Repeating Characters
/// (Variable-length sliding window)
///
/// Input:  channels = {1, 2, 3, 2, 1, 4, 5, 6}
/// Output: 5  (segment {2, 1, 4, 5, 6} or {3, 2, 1, 4, 5, 6} = wrong, {1,4,5,6,2})
/// Actually: {1,2,3,2,...} not repeat = 3, then {2,1,4,5,6} = 5
int longestUniqueSegment(const std::vector<int>& channels);

/// Car scenario: The car should find the shortest sub-array of sensor readings
/// whose sum >= a threshold. Useful for "how many readings to detect obstacle"
///
/// Maps to: LeetCode 209 - Minimum Size Subarray Sum
int minReadingsForThreshold(const std::vector<int>& readings, int threshold);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__VARIABLE_SLIDING_WINDOW_HPP_
