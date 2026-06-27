#ifndef FREENOVE_CAR_CORE__LEETCODE__DETECT_OBJECTS_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__DETECT_OBJECTS_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: Camera processes an image into a binary grid.
/// '1' = obstacle pixel, '0' = free space. Find total number of
/// distinct obstacles (connected components of '1').
///
/// Maps to: LeetCode 200 - Number of Islands (BFS/DFS on Grid)
int countObstacles(std::vector<std::vector<char>>& grid);

/// Bonus: Find the largest obstacle by pixel area.
///
/// Maps to: LeetCode 695 - Max Area of Island
int largestObstacle(std::vector<std::vector<int>>& grid);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__DETECT_OBJECTS_HPP_
