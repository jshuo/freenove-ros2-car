# LeetCode → Freenove Car Kit: Algorithm Mapping

Every LeetCode problem in this package maps to a real embedded robotics scenario on the Freenove car. Run any algorithm, drive the car, and watch the result change with the physical world.

---

## Tier 1: High Frequency (Amazon OA staples)

### LC 1 — Two Sum

**Car scenario:** Onboard firmware fault detection. A rolling buffer of telemetry values (battery voltage, vibration, temperature, servo current, light sensor) is scanned for two readings whose sum equals a known fault threshold.

**Example:** `battery=12, vibration=8` → sum = 20 → bearing wear threshold hit

| Telemetry buffer | Fault threshold | Result |
|---|---|---|
| `{12, 8, 3, 17, 5, 15}` | 20 | `(0, 1)` |
| `{12, 8, 3, 17, 5, 15}` | 99 | `(-1, -1)` (no match) |

**Physical experiment:** Drive the car over a rough surface. Log vibration + ultrasonic readings. Set threshold = (normal value × 2), see if Two Sum fires when you hit a bump.

**Header:** `include/leetcode/sensor_pattern.hpp`
**Source:** `src/leetcode/sensor_pattern.cpp`

---

### LC 3 — Longest Substring Without Repeating Characters

**Car scenario:** The car receives sensor channel IDs from multiplexed peripheral reads. Find the longest stretch where each channel ID appears at most once — i.e., no repeated sensor poll in a row.

**Example:** `channels = {1, 2, 3, 2, 1, 4, 5, 6}` → longest unique segment = 6 (`{3, 2, 1, 4, 5, 6}`)

**Physical experiment:** Have the car poll sensors in round-robin. Inject a duplicate channel, see how the buffer length reacts.

**Header:** `include/leetcode/variable_sliding_window.hpp`
**Source:** `src/leetcode/variable_sliding_window.cpp`

---

### LC 15 — 3Sum

**Car scenario:** The car's diagnostic system scans three independent sensor readings that sum to a target. Useful for detecting composite fault conditions (temperature + vibration + current = overload signature).

**Example:** `readings = {12, 3, 5, 8, 1, 6, 4}`, target 12 → `3 + 5 + 4 = 12` → true

**Physical experiment:** Collect three sensor streams. Compute their sum at each tick. When the sum hits a predetermined target (e.g., normal idle sum), the algorithm should find the combination.

**Header:** `include/leetcode/perfect_measurement.hpp`
**Source:** `src/leetcode/perfect_measurement.cpp`

---

### LC 53 — Maximum Subarray (Kadane's Algorithm)

**Car scenario:** The car's battery monitoring system tracks voltage over time. Find the sub-segment of the drive where battery drain was highest (or voltage recovery was strongest).

**Physical experiment:** Drive the car up a ramp (high drain) then coast down (regen/coast). Kadane should identify the ramp segment as the max subarray.

**Note:** This maps to the same `sliding_window` file as the safe-path algorithm. The pattern is: find the contiguous segment with the maximum "something" in a stream.

**Header:** `include/leetcode/safe_path.hpp` (extended)
**Source:** `src/leetcode/safe_path.cpp`

---

### LC 200 — Number of Islands
### LC 695 — Max Area of Island

**Car scenario:** The camera captures a binary occupancy grid (`1` = obstacle pixel, `0` = free space). Count connected obstacle blobs (Number of Islands). Also find the largest obstacle by pixel count (Max Area).

**Example:**
```
Grid:           Obstacles:
1 1 0 0 0       A A . . .
1 1 0 0 0       A A . . .
0 0 1 0 0       . . B . .
0 0 0 1 1       . . . C C
```
count = 3 (A, B, C)

**Physical experiment:** Place 3-5 objects on a dark floor, take a top-down photo, threshold to binary, run the algorithm. Count should match the objects you placed.

**Header:** `include/leetcode/detect_objects.hpp`
**Source:** `src/leetcode/detect_objects.cpp`

---

### LC 209 — Minimum Size Subarray Sum

**Car scenario:** The car needs to detect an obstacle reliably. How many consecutive sensor readings must be above a threshold before we're confident an obstacle is present? Find the shortest subarray whose sum ≥ confidence threshold.

**Example:** `readings = {2, 3, 1, 2, 4, 3}`, confidence threshold = 7 → minimum 2 readings (`{3, 4}` or `{4, 3}`)

**Physical experiment:** Place the car facing a wall. Slowly move it closer. The algorithm should report decreasing segment lengths as more readings exceed the safety threshold.

**Header:** `include/leetcode/variable_sliding_window.hpp`
**Source:** `src/leetcode/variable_sliding_window.cpp`

---

### LC 560 — Subarray Sum Equals K
### LC 325 — Maximum Size Subarray Sum Equals K

**Car scenario:** The car logs voltage over time. Count how many continuous segments have total voltage exactly equal to a target (LC 560). Also find the longest such segment (LC 325). Useful for identifying specific power-consumption patterns.

**Example:** `readings = {1, 2, 3, -2, 5}`, target = 5
- Subarrays summing to 5: `[2, 3]` and `[5]` = 2 segments
- Longest segment summing to 5: `[2, 3]` = length 2

**Physical experiment:** Log battery voltage during a drive cycle. Target = average idle consumption. Count how many segments match the idle profile.

**Header:** `include/leetcode/prefix_sum_hashing.hpp`
**Source:** `src/leetcode/prefix_sum_hashing.cpp`

---

### LC 704 — Binary Search

**Car scenario:** The car has a sorted log of sensor timestamps. Find the first timestamp where a reading exceeds a critical threshold. O(log n) instead of scanning the whole log.

**Example:** `sorted_readings = {10, 20, 30, 40, 50, 60, 70}`, threshold = 35 → index 3 (value 40)

**Physical experiment:** Sort the car's ultrasonic readings, then binary-search for "first reading below 15 cm" (imminent collision).

**Header:** `include/leetcode/binary_search.hpp`
**Source:** `src/leetcode/binary_search.cpp`

---

### LC 739 — Daily Temperatures

**Car scenario:** The car's IR sensor array reads reflectivity at positions along its path. For each position, find the distance (number of steps) to the next higher reflectivity reading (a closer obstacle). `0` means no higher reading ahead.

**Example:** `reflectivity = {73, 74, 75, 71, 69, 72, 76, 73}`
→ `distances = {1, 1, 4, 2, 1, 1, 0, 0}`
(position 2: next higher reading is 4 steps ahead at index 6)

**Physical experiment:** Place objects at increasing distances from the car. Drive forward. The algorithm should report the "steps to next obstacle" at each tick.

**Header:** `include/leetcode/obstacle_ahead.hpp`
**Source:** `src/leetcode/obstacle_ahead.cpp`

---

### LC 875 — Koko Eating Bananas (Binary Search on Answer)

**Car scenario:** The car needs to complete a route with segments of varying distances within a time limit. What is the minimum speed that allows finishing on time? Binary search on speed.

**Example:** `distances = {30, 11, 23, 4, 20}`, time limit = 6 hours
→ minimum speed = 23 units/hour

**Physical experiment:** Mark a course with segments of different lengths. Set a "battery budget" time limit. The algorithm computes the minimum speed to finish before the battery dies.

**Header:** `include/leetcode/binary_search.hpp`
**Source:** `src/leetcode/binary_search.cpp`

---

## Tier 2: Medium-High Frequency

---

### LC 141 — Linked List Cycle Detection

**Car scenario:** The car records every visited waypoint as a linked list of `Position` nodes. If the car ever revisits a previous position (navigation loop), Floyd's Tortoise and Hare detects the cycle in O(n) time, O(1) space.

**Example:** `(1,2) → (3,5) → (7,1) → (2,8) → (3,5)` (cycle back to position B)
→ cycle detected = true

**Physical experiment:** Drive the car in a circle and log positions. The algorithm should detect the path repeating.

**Header:** `include/leetcode/path_history.hpp`
**Source:** `src/leetcode/path_history.cpp`

---

### LC 206 — Reverse Linked List

**Car scenario:** The car needs to return to its starting point. Reverse the path history in-place to compute the return route.

**Example:** `(1,1) → (2,2) → (3,3)` → reversed: `(3,3) → (2,2) → (1,1)`

**Physical experiment:** Drive an L-shaped path. Reverse the log and verify the return path is the opposite.

**Header:** `include/leetcode/path_history.hpp`
**Source:** `src/leetcode/path_history.cpp`

---

### LC 21 — Merge Two Sorted Lists

**Car scenario:** Two separate drive logs (morning and afternoon) are each sorted by timestamp. Merge them into one chronological log.

**Example:** `[1→4→7] + [2→5→8]` → `[1→2→4→5→7→8]`

**Physical experiment:** Drive two separate routes. Merge the logs and verify timestamps are in order.

**Header:** `include/leetcode/path_history.hpp`
**Source:** `src/leetcode/path_history.cpp`

---

### LC 876 — Middle of the Linked List

**Car scenario:** Find the midpoint of the recorded path. Useful for splitting a long route into two segments for battery management.

**Example:** `[1→2→4→5→7→8]` → middle = node with value 5

**Physical experiment:** Drive a path with an odd number of waypoints. The middle node should be the halfway point geographically.

**Header:** `include/leetcode/path_history.hpp`
**Source:** `src/leetcode/path_history.cpp`

### LC 23 — Merge k Sorted Lists

**Car scenario:** Multiple sensors each produce a sorted timestamp stream. Merge them into one chronological log for post-drive diagnostics. Detect which sensor lags behind the others.

**Example:** Sensor A = `{1, 4, 7}`, Sensor B = `{2, 5, 8}`, Sensor C = `{3, 6, 9}`
→ merged = `{1, 2, 3, 4, 5, 6, 7, 8, 9}`

**Physical experiment:** Log three sensor topics (`/sensor/state`, `/battery/voltage`, `/motor/encoder`). Merge into a single sorted CSV. Verify chronological order.

**Header:** `include/leetcode/top_k_heap.hpp`
**Source:** `src/leetcode/top_k_heap.cpp`

---

### LC 347 — Top K Frequent Elements

**Car scenario:** The car logs thousands of sensor readings. Which k readings occur most frequently? Identifies the dominant sensor state values (e.g., "mostly forward-moving at 30% throttle").

**Example:** `readings = {1,1,1,2,2,3,4,4,4,4,5}`, k = 3
→ Top 3: `{4, 1, 2}` (value 4 appears 4×, value 1 appears 3×, value 2 appears 2×)

**Physical experiment:** Drive the car in a circle (repeated left turn). The top-k should show `LEFT` as the dominant steering command. Then drive straight and see it shift to `FORWARD`.

**Header:** `include/leetcode/top_k_heap.hpp`
**Source:** `src/leetcode/top_k_heap.cpp`

---

## Running All Algorithms

```bash
cd ~/workspaces/ros-ws
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.sh

# Run all tests with simulated data
ros2 run freenove_car_core leetcode_node --ros-args -p simulate:=true

# Run on real car (subscribes to /sensor/state)
ros2 run freenove_car_core leetcode_node
```

## Experiment Log Template

For each physical experiment, log:

```csv
timestamp,algorithm,input_value,output,expected,passed
```

The `leetcode_node` already prints structured output — pipe it to a file:

```bash
ros2 run freenove_car_core leetcode_node --ros-args -p simulate:=true \
  2>&1 | grep "LC" > experiment_log.csv
```

## File Index

| Header (.hpp) | Source (.cpp) | LC Problems Covered |
|---|---|---|
| `sensor_pattern.hpp` | `sensor_pattern.cpp` | 1 |
| `safe_path.hpp` | `safe_path.cpp` | 53 |
| `variable_sliding_window.hpp` | `variable_sliding_window.cpp` | 3, 209 |
| `perfect_measurement.hpp` | `perfect_measurement.cpp` | 15 |
| `obstacle_ahead.hpp` | `obstacle_ahead.cpp` | 739 |
| `detect_objects.hpp` | `detect_objects.cpp` | 200, 695 |
| `binary_search.hpp` | `binary_search.cpp` | 704, 875 |
| `prefix_sum_hashing.hpp` | `prefix_sum_hashing.cpp` | 560, 325 |
| `top_k_heap.hpp` | `top_k_heap.cpp` | 347, 23 |
| `path_history.hpp` | `path_history.cpp` | 141, 206, 21, 876 |
