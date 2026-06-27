#include "freenove_car_core/leetcode/binary_search.hpp"
#include <algorithm>
#include <cmath>
#include <climits>

namespace freenove_car_core {
namespace leetcode {

int firstExceeds(const std::vector<int>& sorted, int threshold) {
  int lo = 0, hi = static_cast<int>(sorted.size()) - 1;
  int ans = -1;
  while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (sorted[mid] > threshold) {
      ans = mid;
      hi = mid - 1;  // look left for first exceeding
    } else {
      lo = mid + 1;  // look right
    }
  }
  return ans;  // index, or -1 if none
}

int minSpeedToFinish(const std::vector<int>& distances, int time_limit) {
  // Binary search on speed: find minimum speed such that
  // total_time = sum(ceil(dist / speed)) <= time_limit
  int lo = 1, hi = *std::max_element(distances.begin(), distances.end());
  int ans = hi;

  while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    long long total_time = 0;
    for (int d : distances) {
      total_time += static_cast<long long>(d + mid - 1) / mid;  // ceil
    }
    if (total_time <= time_limit) {
      ans = mid;
      hi = mid - 1;
    } else {
      lo = mid + 1;
    }
  }
  return ans;
}

}  // namespace leetcode
}  // namespace freenove_car_core
