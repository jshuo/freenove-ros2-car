#ifndef FREENOVE_CAR_CORE__LEETCODE__PATH_HISTORY_HPP_
#define FREENOVE_CAR_CORE__LEETCODE__PATH_HISTORY_HPP_

namespace freenove_car_core {
namespace leetcode {

/// Car scenario: The car records every visited waypoint as a linked list
/// of Position nodes. Detect if the car ever revisits the same position
/// (got stuck in a navigation loop).
///
/// Maps to: LeetCode 141 - Linked List Cycle Detection
/// (Floyd's Tortoise and Hare, O(n) time, O(1) space)
struct Position {
  int x;
  int y;
  Position* next;
  Position(int x_, int y_, Position* next_ = nullptr)
      : x(x_), y(y_), next(next_) {}
};

bool hasCycle(Position* head);

/// Car scenario: The car needs to reverse its path to return to base.
/// Reverse the path history in place.
///
/// Maps to: LeetCode 206 - Reverse Linked List
Position* reversePath(Position* head);

/// Car scenario: Merge two recorded paths (e.g., morning run + afternoon run)
/// sorted by timestamp into one chronological path.
///
/// Maps to: LeetCode 21 - Merge Two Sorted Lists
Position* mergePaths(Position* a, Position* b);

/// Car scenario: Find the midpoint of the path — useful for splitting
/// a long route into two segments for battery management.
///
/// Maps to: LeetCode 876 - Middle of the Linked List
Position* middleOfPath(Position* head);

}  // namespace leetcode
}  // namespace freenove_car_core

#endif  // FREENOVE_CAR_CORE__LEETCODE__PATH_HISTORY_HPP_
