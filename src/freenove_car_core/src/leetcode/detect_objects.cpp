#include "freenove_car_core/leetcode/detect_objects.hpp"
#include <queue>
#include <algorithm>
#include <utility>

namespace freenove_car_core {
namespace leetcode {

namespace {
const int DIRS[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void bfsMarkChar(std::vector<std::vector<char>>& grid, int r, int c,
                  int rows, int cols) {
  std::queue<std::pair<int, int>> q;
  q.push({r, c});
  grid[r][c] = '0';
  while (!q.empty()) {
    auto [cr, cc] = q.front(); q.pop();
    for (auto& d : DIRS) {
      int nr = cr + d[0], nc = cc + d[1];
      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == '1') {
        grid[nr][nc] = '0';
        q.push({nr, nc});
      }
    }
  }
}

int bfsAreaInt(std::vector<std::vector<int>>& grid, int r, int c,
               int rows, int cols) {
  std::queue<std::pair<int, int>> q;
  q.push({r, c});
  grid[r][c] = 0;
  int area = 0;
  while (!q.empty()) {
    auto [cr, cc] = q.front(); q.pop();
    ++area;
    for (auto& d : DIRS) {
      int nr = cr + d[0], nc = cc + d[1];
      if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == 1) {
        grid[nr][nc] = 0;
        q.push({nr, nc});
      }
    }
  }
  return area;
}
}  // namespace

int countObstacles(std::vector<std::vector<char>>& grid) {
  if (grid.empty() || grid[0].empty()) return 0;
  int rows = static_cast<int>(grid.size());
  int cols = static_cast<int>(grid[0].size());
  int count = 0;
  for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
      if (grid[r][c] == '1') {
        ++count;
        bfsMarkChar(grid, r, c, rows, cols);
      }
  return count;
}

int largestObstacle(std::vector<std::vector<int>>& grid) {
  if (grid.empty() || grid[0].empty()) return 0;
  int rows = static_cast<int>(grid.size());
  int cols = static_cast<int>(grid[0].size());
  int max_area = 0;
  for (int r = 0; r < rows; ++r)
    for (int c = 0; c < cols; ++c)
      if (grid[r][c] == 1)
        max_area = std::max(max_area, bfsAreaInt(grid, r, c, rows, cols));
  return max_area;
}

}  // namespace leetcode
}  // namespace freenove_car_core
