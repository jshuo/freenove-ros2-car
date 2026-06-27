#ifndef FREENOVE_CAR_CORE__LEETCODE__OBSTACLE_AHEAD_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__OBSTACLE_AHEAD_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: The car's forward-facing IR array detects reflectivity levels.
/// For each position, find the distance to the next HIGHER reflectivity reading
/// (closer obstacle). 0 means no higher reading ahead.
///
/// Maps to: LeetCode 739 - Daily Temperatures (Monotonic Stack)
///
/// Input:  reflectivity = {73, 74, 75, 71, 69, 72, 76, 73}
/// Output: distances = {1, 1, 4, 2, 1, 1, 0, 0}
///         (position 2: next higher is 4 steps ahead)
std::vector<int> obstacleDistance(const std::vector<int>& reflectivity);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__OBSTACLE_AHEAD_HPP_
