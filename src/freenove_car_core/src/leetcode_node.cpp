/// LeetCode Algorithm Node
/// Subscribes to real car sensor data and runs algorithm exercises as
/// a real-time diagnostic. In simulation mode, uses synthetic data.
///
/// Run: ros2 run freenove_car_core leetcode_node
/// Test: ros2 run freenove_car_core leetcode_node --ros-args -p simulate:=true

#include <cstdio>
#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "freenove_car_msgs/msg/sensor_state.hpp"

#include "freenove_car_core/leetcode/safe_path.hpp"
#include "freenove_car_core/leetcode/perfect_measurement.hpp"
#include "freenove_car_core/leetcode/obstacle_ahead.hpp"
#include "freenove_car_core/leetcode/detect_objects.hpp"
#include "freenove_car_core/leetcode/variable_sliding_window.hpp"
#include "freenove_car_core/leetcode/binary_search.hpp"
#include "freenove_car_core/leetcode/prefix_sum_hashing.hpp"
#include "freenove_car_core/leetcode/top_k_heap.hpp"
#include "freenove_car_core/leetcode/sensor_pattern.hpp"
#include "freenove_car_core/leetcode/path_history.hpp"

namespace freenove_car_core {

class LeetCodeNode : public rclcpp::Node {
public:
  LeetCodeNode()
      : Node("leetcode_node"),
        safe_path_window_(100),
        min_safe_cm_(20) {
    declare_parameter("simulate", false);
    bool simulate = get_parameter("simulate").as_bool();

    if (simulate) {
      RCLCPP_INFO(get_logger(), "Simulation mode — running all exercises");
      // Run tests on a one-shot timer so spin() is active when we shutdown
      run_timer_ = create_wall_timer(
          std::chrono::milliseconds(100),
          std::bind(&LeetCodeNode::runAllTests, this));
    } else {
      sensor_sub_ = create_subscription<freenove_car_msgs::msg::SensorState>(
          "/sensor/state", 10,
          std::bind(&LeetCodeNode::onSensorState, this, std::placeholders::_1));
      RCLCPP_INFO(get_logger(), "Real car mode — subscribing to /sensor/state");
    }
  }

private:
  void onSensorState(const freenove_car_msgs::msg::SensorState::SharedPtr msg) {
    float dist = msg->ultrasonic_distance;
    if (dist > 0) {
      safe_path_window_.push_back(static_cast<int>(dist));
      if (safe_path_window_.size() > 100) safe_path_window_.pop_front();
      std::vector<int> readings(safe_path_window_.begin(), safe_path_window_.end());
      int safe_len = leetcode::longestSafePath(readings, min_safe_cm_);
      RCLCPP_DEBUG(get_logger(), "Live safe path: %d readings", safe_len);
    }
  }

  void runAllTests() {
    if (run_timer_) run_timer_->cancel();  // one-shot

    RCLCPP_INFO(get_logger(), "");
    RCLCPP_INFO(get_logger(), "===================================================");
    RCLCPP_INFO(get_logger(), "   AMAZON OA PREP — ALL LEETCODE PATTERNS ");
    RCLCPP_INFO(get_logger(), "===================================================");
    RCLCPP_INFO(get_logger(), "");

    // ═══════════════════════════════════════════════
    // TIER 1: HIGH FREQUENCY (must pass)
    // ═══════════════════════════════════════════════

    test_fixed_sliding_window();
    test_variable_sliding_window();
    test_two_pointer();
    test_hash_prefix_sum();
    test_monotonic_stack();
    test_bfs_grid();

    // ═══════════════════════════════════════════════
    // TIER 2: MEDIUM-HIGH FREQUENCY
    // ═══════════════════════════════════════════════

    test_binary_search();
    test_top_k_heap();
    test_two_sum();
    test_linked_list();

    RCLCPP_INFO(get_logger(), "");
    RCLCPP_INFO(get_logger(), "===================================================");
    RCLCPP_INFO(get_logger(), "   ALL PATTERNS COVERED — ready for OA!");
    RCLCPP_INFO(get_logger(), "===================================================");
    RCLCPP_INFO(get_logger(), "");

    // Run only once
    rclcpp::shutdown();
  }

  // ──────────── TESTS ────────────

  void test_fixed_sliding_window() {
    std::vector<int> r1 = {50, 60, 15, 80, 90, 100, 5, 70};
    int result = leetcode::longestSafePath(r1, 20);
    RCLCPP_INFO(get_logger(), "FIXED SLIDING WINDOW");
    RCLCPP_INFO(get_logger(), "  LC 1: SafePath (obstacle-free segment) = %d  (expect 3)", result);
    RCLCPP_INFO(get_logger(), "");
  }

  void test_variable_sliding_window() {
    // LC 3 — longest without repeating
    std::vector<int> channels = {1, 2, 3, 2, 1, 4, 5, 6};
    int r1 = leetcode::longestUniqueSegment(channels);
    RCLCPP_INFO(get_logger(), "VARIABLE SLIDING WINDOW");

    // LC 209 — minimum size subarray sum
    std::vector<int> readings = {2, 3, 1, 2, 4, 3};
    int r2 = leetcode::minReadingsForThreshold(readings, 7);

    RCLCPP_INFO(get_logger(), "  LC 3:  LongestUniqueSegment = %d  (expect 6)", r1);
    RCLCPP_INFO(get_logger(), "  LC 209: MinReadingsForSum>=7 = %d  (expect 2)", r2);
    RCLCPP_INFO(get_logger(), "");
  }

  void test_two_pointer() {
    std::vector<int> readings = {12, 3, 5, 8, 1, 6, 4};
    bool r1 = leetcode::hasPerfectMeasurement(readings, 12);
    bool r2 = leetcode::hasPerfectMeasurement(readings, 99);
    RCLCPP_INFO(get_logger(), "TWO POINTER / 3SUM");
    RCLCPP_INFO(get_logger(), "  LC 15: 3 readings sum to 12 = %s  (expect true)",  r1 ? "true" : "false");
    RCLCPP_INFO(get_logger(), "  LC 15: 3 readings sum to 99 = %s (expect false)", r2 ? "true" : "false");
    RCLCPP_INFO(get_logger(), "");
  }

  void test_hash_prefix_sum() {
    // [1,2,3,-2,5]: subarrays summing to 5 = [2,3] and [5] => 2
    // longest subarray summing to 5 = [2,3] => len 2
    std::vector<int> readings = {1, 2, 3, -2, 5};
    int c1 = leetcode::countSegmentsWithSum(readings, 5);
    int c2 = leetcode::longestSegmentWithSum(readings, 5);
    RCLCPP_INFO(get_logger(), "HASH + PREFIX SUM");
    RCLCPP_INFO(get_logger(), "  LC 560: CountSubarraysSum=5 = %d (expect 2)",  c1);
    RCLCPP_INFO(get_logger(), "  LC 325: LongestSubarraySum=5 = %d (expect 2)", c2);

    // Better test: all positive = easier to verify
    std::vector<int> readings2 = {1, 1, 1};
    int c3 = leetcode::countSegmentsWithSum(readings2, 2);
    int c4 = leetcode::longestSegmentWithSum(readings2, 2);
    RCLCPP_INFO(get_logger(), "  LC 560 (simple): Count [1,1,1] sum=2 = %d (expect 2)", c3);
    RCLCPP_INFO(get_logger(), "  LC 325 (simple): Longest [1,1,1] sum=2 = %d (expect 2)", c4);
    RCLCPP_INFO(get_logger(), "");
  }

  void test_monotonic_stack() {
    std::vector<int> readings = {73, 74, 75, 71, 69, 72, 76, 73};
    auto dist = leetcode::obstacleDistance(readings);
    std::string s;
    for (auto d : dist) s += std::to_string(d) + " ";
    RCLCPP_INFO(get_logger(), "MONOTONIC STACK");
    RCLCPP_INFO(get_logger(), "  LC 739: ObstacleDistance = %s (expect 1 1 4 2 1 1 0 0)", s.c_str());
    RCLCPP_INFO(get_logger(), "");
  }

  void test_bfs_grid() {
    std::vector<std::vector<char>> g1 = {
      {'1','1','0','0','0'},
      {'1','1','0','0','0'},
      {'0','0','1','0','0'},
      {'0','0','0','1','1'}
    };
    int c = leetcode::countObstacles(g1);
    RCLCPP_INFO(get_logger(), "BFS / DFS GRID");
    RCLCPP_INFO(get_logger(), "  LC 200: CountObstacles = %d  (expect 3)", c);

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
    int a = leetcode::largestObstacle(g2);
    RCLCPP_INFO(get_logger(), "  LC 695: LargestObstacle = %d (expect 6)", a);
    RCLCPP_INFO(get_logger(), "");
  }

  void test_binary_search() {
    std::vector<int> sorted = {10, 20, 30, 40, 50, 60, 70};
    int idx = leetcode::firstExceeds(sorted, 35);
    std::vector<int> dists = {30, 11, 23, 4, 20};
    int speed = leetcode::minSpeedToFinish(dists, 6);
    RCLCPP_INFO(get_logger(), "BINARY SEARCH");
    RCLCPP_INFO(get_logger(), "  LC 704: First reading >35 at index %d  (expect 3)",  idx);
    RCLCPP_INFO(get_logger(), "  LC 875: Min speed for 6h = %d (expect 23)", speed);
    RCLCPP_INFO(get_logger(), "");
  }

  void test_top_k_heap() {
    std::vector<int> readings = {1, 1, 1, 2, 2, 3, 4, 4, 4, 4, 5};
    auto top = leetcode::topKFrequentReadings(readings, 3);
    std::string s;
    for (auto v : top) s += std::to_string(v) + " ";
    RCLCPP_INFO(get_logger(), "HEAP / PRIORITY QUEUE");
    RCLCPP_INFO(get_logger(), "  LC 347: Top 3 frequent = %s", s.c_str());

    std::vector<std::vector<int>> logs = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
    auto merged = leetcode::mergeSortedLogs(logs);
    std::string m;
    for (auto v : merged) m += std::to_string(v) + " ";
    RCLCPP_INFO(get_logger(), "  LC 23: Merged logs = %s", m.c_str());
    RCLCPP_INFO(get_logger(), "");
  }

  void test_linked_list() {
    using leetcode::Position;

    // LC 141: Build list with a cycle
    Position* p1 = new Position(1, 2);
    Position* p2 = new Position(3, 5);
    Position* p3 = new Position(7, 1);
    Position* p4 = new Position(2, 8);
    p1->next = p2; p2->next = p3; p3->next = p4; p4->next = p2;  // cycle p4→p2
    bool cycle = leetcode::hasCycle(p1);

    // LC 206: Reverse a non-cyclic list
    Position* a = new Position(1, 1);
    Position* b = new Position(2, 2);
    Position* c = new Position(3, 3);
    a->next = b; b->next = c;
    Position* reversed = leetcode::reversePath(a);
    int rev_x = reversed->x;  // should be 3

    // LC 21: Merge sorted by x
    Position* list1 = new Position(1, 0, new Position(4, 0, new Position(7, 0)));
    Position* list2 = new Position(2, 0, new Position(5, 0, new Position(8, 0)));
    Position* merged = leetcode::mergePaths(list1, list2);
    int m1 = merged->x, m2 = merged->next->x;

    // LC 876: Middle
    Position* mid = leetcode::middleOfPath(merged);
    int mid_x = mid->x;

    RCLCPP_INFO(get_logger(), "LINKED LIST (path history)");
    RCLCPP_INFO(get_logger(), "  LC 141: Cycle detect = %s (expect true)",  cycle ? "true" : "false");
    RCLCPP_INFO(get_logger(), "  LC 206: Reverse head x = %d (expect 3)", rev_x);
    RCLCPP_INFO(get_logger(), "  LC 21:  Merge first two = %d,%d (expect 1,2)", m1, m2);
    RCLCPP_INFO(get_logger(), "  LC 876: Middle x = %d (expect 5)", mid_x);
    RCLCPP_INFO(get_logger(), "");

    // Cleanup (skip cycle nodes to avoid infinite loop)
    delete p3; delete p4;
    delete reversed; delete reversed->next;
    delete merged->next->next->next;
  }

  void test_two_sum() {
    // Telemetry: battery=12, vibration=8, temp=3, current=17, servo=5, light=15
    std::vector<int> telemetry = {12, 8, 3, 17, 5, 15};
    auto r1 = leetcode::findFaultPattern(telemetry, 20);
    auto r2 = leetcode::findFaultPattern(telemetry, 99);
    RCLCPP_INFO(get_logger(), "TWO SUM (sensor fault detection)");
    RCLCPP_INFO(get_logger(), "  LC 1: Fault target=20 -> (%d,%d) (expect (0,1))", r1.first, r1.second);
    RCLCPP_INFO(get_logger(), "  LC 1: Fault target=99 -> (%d,%d) (expect (-1,-1))", r2.first, r2.second);
    RCLCPP_INFO(get_logger(), "");
  }

  rclcpp::Subscription<freenove_car_msgs::msg::SensorState>::SharedPtr sensor_sub_;
  rclcpp::TimerBase::SharedPtr run_timer_;
  std::deque<int> safe_path_window_;
  int min_safe_cm_;
};

}  // namespace freenove_car_core

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<freenove_car_core::LeetCodeNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
