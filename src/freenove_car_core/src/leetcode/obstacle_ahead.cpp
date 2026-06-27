#include "freenove_car_core/leetcode/obstacle_ahead.hpp"
#include <stack>

namespace freenove_car_core {
namespace leetcode {

std::vector<int> obstacleDistance(const std::vector<int>& readings) {
  int n = static_cast<int>(readings.size());
  std::vector<int> result(n, 0);
  std::stack<int> st;

  for (int i = 0; i < n; ++i) {
    while (!st.empty() && readings[i] > readings[st.top()]) {
      result[st.top()] = i - st.top();
      st.pop();
    }
    st.push(i);
  }
  return result;
}

}  // namespace leetcode
}  // namespace freenove_car_core
