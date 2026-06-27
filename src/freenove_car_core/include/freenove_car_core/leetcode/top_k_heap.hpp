#ifndef FREENOVE_CAR_CORE__LEETCODE__TOP_K_HEAP_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__TOP_K_HEAP_HPP_

#include <vector>

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: The car logs sensor readings. Find the k most-frequent
/// sensor values. (Useful for detecting which readings dominate)
///
/// Maps to: LeetCode 347 - Top K Frequent Elements
/// (Heap / Bucket sort)
std::vector<int> topKFrequentReadings(const std::vector<int>& readings, int k);

/// Car scenario: Given k streams of timestamp data (each sorted),
/// merge them into one sorted log. Diagnose which sensor lags.
///
/// Maps to: LeetCode 23 - Merge k Sorted Lists
/// (Priority queue / min-heap)
std::vector<int> mergeSortedLogs(const std::vector<std::vector<int>>& logs);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__TOP_K_HEAP_HPP_
