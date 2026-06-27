#include "freenove_car_core/leetcode/sensor_pattern.hpp"
#include <unordered_map>
#include <utility>

namespace freenove_car_core {
namespace leetcode {

std::pair<int, int> findFaultPattern(const std::vector<int>& telemetry,
                                     int fault_threshold) {
  // One pass, O(n) with hash map — exact Two Sum
  std::unordered_map<int, int> seen;  // value -> index
  seen.reserve(telemetry.size());

  for (int i = 0; i < static_cast<int>(telemetry.size()); ++i) {
    int complement = fault_threshold - telemetry[i];
    auto it = seen.find(complement);
    if (it != seen.end()) {
      return {it->second, i};
    }
    seen[telemetry[i]] = i;
  }
  return {-1, -1};
}

}  // namespace leetcode
}  // namespace freenove_car_core
