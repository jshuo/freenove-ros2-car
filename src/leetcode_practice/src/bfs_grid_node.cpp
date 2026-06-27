#include <cstdio>
#include "leetcode_practice/bfs_grid.hpp"

int main() {
  std::vector<std::vector<char>> g1 = {
    {'1','1','1','1','0'},
    {'1','1','0','1','0'},
    {'1','1','0','0','0'},
    {'0','0','0','0','0'}
  };
  printf("Number of Islands: %d\n", leetcode_practice::numIslands(g1));

  std::vector<std::vector<int>> g2 = {
    {0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0},
    {0,1,1,0,1,0,0,0,0,0,0,0,0},
    {0,1,0,0,1,1,0,0,1,0,1,0,0},
    {0,1,0,0,1,1,0,0,1,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0}
  };
  printf("Max Area of Island: %d\n", leetcode_practice::maxAreaOfIsland(g2));
  return 0;
}
