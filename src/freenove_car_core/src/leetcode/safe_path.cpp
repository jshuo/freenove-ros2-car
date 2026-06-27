#include "freenove_car_core/leetcode/safe_path.hpp"
#include <algorithm>

namespace freenove_car_core {
namespace leetcode {

int longestSafePath(const std::vector<int>& readings, int min_safe_cm) {
  // Variable-length sliding window
  // Keep expanding right; if reading < min_safe, move left past it
  int left = 0, max_len = 0;
  for (int right = 0; right < static_cast<int>(readings.size()); ++right) {
    if (readings[right] < min_safe_cm) {
      left = right + 1;  // obstacle found, restart window
    } else {
      max_len = std::max(max_len, right - left + 1);
    }
  }
  return max_len;
}

}  // namespace leetcode
}  // namespace freenove_car_core
