#include "leetcode_practice/monotonic_stack.hpp"
#include <stack>

namespace leetcode_practice {

std::vector<int> nextGreaterElement(const std::vector<int>& nums) {
  int n = static_cast<int>(nums.size());
  std::vector<int> result(n, -1);
  std::stack<int> st;
  for (int i = 0; i < n; ++i) {
    while (!st.empty() && nums[i] > nums[st.top()]) {
      result[st.top()] = nums[i];
      st.pop();
    }
    st.push(i);
  }
  return result;
}

std::vector<int> dailyTemperatures(const std::vector<int>& temperatures) {
  int n = static_cast<int>(temperatures.size());
  std::vector<int> result(n, 0);
  std::stack<int> st;
  for (int i = 0; i < n; ++i) {
    while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
      result[st.top()] = i - st.top();
      st.pop();
    }
    st.push(i);
  }
  return result;
}

}  // namespace leetcode_practice
