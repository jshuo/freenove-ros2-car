#include <cassert>
#include <cstdio>
#include "leetcode_practice/array_two_sum.hpp"
#include "leetcode_practice/sliding_window.hpp"
#include "leetcode_practice/monotonic_stack.hpp"
#include "leetcode_practice/bfs_grid.hpp"

static void test_two_sum() {
  auto r1 = leetcode_practice::twoSum({2, 7, 11, 15}, 9);
  assert(r1.size() == 2 && ((r1[0] == 0 && r1[1] == 1) || (r1[0] == 1 && r1[1] == 0)));

  auto r2 = leetcode_practice::twoSum({3, 2, 4}, 6);
  assert(r2.size() == 2 && ((r2[0] == 1 && r2[1] == 2) || (r2[0] == 2 && r2[1] == 1)));

  auto r3 = leetcode_practice::twoSum({3, 3}, 6);
  assert(r3.size() == 2);

  auto r4 = leetcode_practice::twoSum({-1, -2, -3, -4, -5}, -8);
  assert(r4.size() == 2);

  printf("  two_sum: PASS\n");
}

static void test_sliding_window() {
  assert(leetcode_practice::maxSubArray({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
  assert(leetcode_practice::maxSubArray({1}) == 1);
  assert(leetcode_practice::maxSubArray({5, 4, -1, 7, 8}) == 23);
  assert(leetcode_practice::maxSubArray({-1}) == -1);

  assert(leetcode_practice::maxSumSubarray({1, 3, -1, -3, 5, 3, 6, 7}, 3) == 16);
  assert(leetcode_practice::maxSumSubarray({1, 2, 3, 4, 5}, 2) == 9);
  assert(leetcode_practice::maxSumSubarray({-1, -2, -3}, 2) == -3);

  printf("  sliding_window: PASS\n");
}

static void test_monotonic_stack() {
  auto r1 = leetcode_practice::nextGreaterElement({2, 1, 2, 4, 3});
  assert((r1 == std::vector<int>{4, 2, 4, -1, -1}));

  auto r2 = leetcode_practice::nextGreaterElement({1, 2, 3, 4, 5});
  assert((r2 == std::vector<int>{2, 3, 4, 5, -1}));

  auto r3 = leetcode_practice::nextGreaterElement({5, 4, 3, 2, 1});
  assert((r3 == std::vector<int>{-1, -1, -1, -1, -1}));

  auto r4 = leetcode_practice::dailyTemperatures({73, 74, 75, 71, 69, 72, 76, 73});
  assert((r4 == std::vector<int>{1, 1, 4, 2, 1, 1, 0, 0}));

  auto r5 = leetcode_practice::dailyTemperatures({30, 40, 50, 60});
  assert((r5 == std::vector<int>{1, 1, 1, 0}));

  printf("  monotonic_stack: PASS\n");
}

static void test_bfs_grid() {
  std::vector<std::vector<char>> g1 = {
    {'1','1','1','1','0'},
    {'1','1','0','1','0'},
    {'1','1','0','0','0'},
    {'0','0','0','0','0'}
  };
  assert(leetcode_practice::numIslands(g1) == 1);

  std::vector<std::vector<char>> g2 = {
    {'1','1','0','0','0'},
    {'1','1','0','0','0'},
    {'0','0','1','0','0'},
    {'0','0','0','1','1'}
  };
  assert(leetcode_practice::numIslands(g2) == 3);

  std::vector<std::vector<int>> g3 = {
    {0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0},
    {0,1,1,0,1,0,0,0,0,0,0,0,0},
    {0,1,0,0,1,1,0,0,1,0,1,0,0},
    {0,1,0,0,1,1,0,0,1,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0}
  };
  assert(leetcode_practice::maxAreaOfIsland(g3) == 6);

  std::vector<std::vector<int>> g4 = {{0, 0}, {0, 0}};
  assert(leetcode_practice::maxAreaOfIsland(g4) == 0);

  printf("  bfs_grid: PASS\n");
}

int main() {
  printf("LeetCode Practice C++ Tests\n");
  printf("==========================\n\n");
  test_two_sum();
  test_sliding_window();
  test_monotonic_stack();
  test_bfs_grid();
  printf("\nAll tests passed!\n");
  return 0;
}
