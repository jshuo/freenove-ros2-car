#include "freenove_car_core/leetcode/perfect_measurement.hpp"
#include <algorithm>

namespace freenove_car_core {
namespace leetcode {

bool hasPerfectMeasurement(const std::vector<int>& readings, int target) {
  // Two-pointer approach (similar to 3Sum variant)
  std::vector<int> sorted(readings);
  std::sort(sorted.begin(), sorted.end());

  int n = static_cast<int>(sorted.size());
  for (int i = 0; i < n - 2; ++i) {
    int left = i + 1;
    int right = n - 1;
    while (left < right) {
      int sum = sorted[i] + sorted[left] + sorted[right];
      if (sum == target) return true;
      if (sum < target) ++left;
      else --right;
    }
  }
  return false;
}

}  // namespace leetcode
}  // namespace freenove_car_core
