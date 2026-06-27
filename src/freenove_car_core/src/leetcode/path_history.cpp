#include "freenove_car_core/leetcode/path_history.hpp"

namespace freenove_car_core {
namespace leetcode {

bool hasCycle(Position* head) {
  if (!head || !head->next) return false;
  Position* slow = head;
  Position* fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) return true;
  }
  return false;
}

Position* reversePath(Position* head) {
  Position* prev = nullptr;
  Position* curr = head;
  while (curr) {
    Position* next_temp = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next_temp;
  }
  return prev;
}

Position* mergePaths(Position* a, Position* b) {
  Position dummy(0, 0);
  Position* tail = &dummy;
  while (a && b) {
    if (a->x <= b->x) {  // sort by x coordinate as timestamp proxy
      tail->next = a;
      a = a->next;
    } else {
      tail->next = b;
      b = b->next;
    }
    tail = tail->next;
  }
  tail->next = a ? a : b;
  return dummy.next;
}

Position* middleOfPath(Position* head) {
  if (!head) return nullptr;
  Position* slow = head;
  Position* fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }
  return slow;
}

}  // namespace leetcode
}  // namespace freenove_car_core
