#include "leetcode_practice/bfs_grid.hpp"
#include <queue>
#include <algorithm>
#include <utility>

namespace leetcode_practice {

namespace {
const int DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void bfsMark(std::vector<std::vector<char>>& grid, int r, int c,
             int rows, int cols) {
  std::queue<std::pair<int, int>> q;
  q.push({r, c});
  grid[r][c] = '0';
  while (!q.empty()) {
    int cr = q.front().first;
    int cc = q.front().second;
    q.pop();
    for (const auto& d : DIRS) {
      int nr = cr + d[0];
      int nc = cc + d[1];
      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == '1') {
        grid[nr][nc] = '0';
        q.push({nr, nc});
      }
    }
  }
}
}  // namespace

int numIslands(std::vector<std::vector<char>>& grid) {
  if (grid.empty() || grid[0].empty()) return 0;
  int rows = static_cast<int>(grid.size());
  int cols = static_cast<int>(grid[0].size());
  int count = 0;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (grid[r][c] == '1') {
        ++count;
        bfsMark(grid, r, c, rows, cols);
      }
    }
  }
  return count;
}

int maxAreaOfIsland(std::vector<std::vector<int>>& grid) {
  if (grid.empty() || grid[0].empty()) return 0;
  int rows = static_cast<int>(grid.size());
  int cols = static_cast<int>(grid[0].size());
  int max_area = 0;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (grid[r][c] == 1) {
        int area = 0;
        std::queue<std::pair<int, int>> q;
        q.push({r, c});
        grid[r][c] = 0;
        while (!q.empty()) {
          int cr = q.front().first;
          int cc = q.front().second;
          q.pop();
          ++area;
          for (const auto& d : DIRS) {
            int nr = cr + d[0];
            int nc = cc + d[1];
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == 1) {
              grid[nr][nc] = 0;
              q.push({nr, nc});
            }
          }
        }
        max_area = std::max(max_area, area);
      }
    }
  }
  return max_area;
}

}  // namespace leetcode_practice
