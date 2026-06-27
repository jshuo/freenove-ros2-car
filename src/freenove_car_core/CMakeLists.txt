cmake_minimum_required(VERSION 3.8)
project(freenove_car_core)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(freenove_car_msgs REQUIRED)

# ==================== LeetCode Algorithm Library ====================
add_library(car_leetcode_algo
  src/leetcode/safe_path.cpp
  src/leetcode/perfect_measurement.cpp
  src/leetcode/obstacle_ahead.cpp
  src/leetcode/detect_objects.cpp
  src/leetcode/variable_sliding_window.cpp
  src/leetcode/binary_search.cpp
  src/leetcode/prefix_sum_hashing.cpp
  src/leetcode/top_k_heap.cpp
  src/leetcode/sensor_pattern.cpp
  src/leetcode/path_history.cpp
)
target_include_directories(car_leetcode_algo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(car_leetcode_algo PUBLIC cxx_std_14)

# ==================== Hardware Library ====================
add_library(freenove_car_hw
  src/pca9685.cpp
  src/motor_controller.cpp
  src/servo_controller.cpp
  src/ultrasonic.cpp
  src/infrared.cpp
  src/adc.cpp
  src/buzzer.cpp
  src/led_controller.cpp
)
target_include_directories(freenove_car_hw PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(freenove_car_hw PUBLIC cxx_std_17)

# ==================== ROS 2 Nodes ====================
# Base node (motors + servos)
add_executable(base_node src/base_node.cpp)
target_link_libraries(base_node freenove_car_hw)
ament_target_dependencies(base_node rclcpp geometry_msgs freenove_car_msgs)
target_include_directories(base_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# Sensor node
add_executable(sensor_node src/sensor_node.cpp)
target_link_libraries(sensor_node freenove_car_hw)
ament_target_dependencies(sensor_node rclcpp std_msgs freenove_car_msgs)
target_include_directories(sensor_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# Actuator node (buzzer + LEDs)
add_executable(actuator_node src/actuator_node.cpp)
target_link_libraries(actuator_node freenove_car_hw)
ament_target_dependencies(actuator_node rclcpp std_msgs freenove_car_msgs)
target_include_directories(actuator_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# Camera node
add_executable(camera_node src/camera_node.cpp)
ament_target_dependencies(camera_node rclcpp sensor_msgs)
target_include_directories(camera_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# ==== ROS 2 Node: LeetCode algorithm debug / validation ====
add_executable(leetcode_node src/leetcode_node.cpp)
target_link_libraries(leetcode_node car_leetcode_algo)
ament_target_dependencies(leetcode_node rclcpp std_msgs freenove_car_msgs)
target_include_directories(leetcode_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# ==================== Install ====================
install(TARGETS
  freenove_car_hw
  car_leetcode_algo
  base_node sensor_node actuator_node camera_node
  leetcode_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/ DESTINATION include)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
