cmake_minimum_required(VERSION 3.8)
project(leetcode_practice)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# ---- Library: all solution implementations ----
add_library(leetcode_solutions
  src/array_two_sum.cpp
  src/sliding_window.cpp
  src/monotonic_stack.cpp
  src/bfs_grid.cpp
)
target_include_directories(leetcode_solutions PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(leetcode_solutions PUBLIC cxx_std_14)

# ---- ROS 2 Node executables ----
function(add_leetcode_node NAME)
  add_executable(${NAME} src/${NAME}_node.cpp)
  target_link_libraries(${NAME} leetcode_solutions)
  target_include_directories(${NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
  install(TARGETS ${NAME} DESTINATION lib/${PROJECT_NAME})
endfunction()

add_leetcode_node(array_two_sum)
add_leetcode_node(sliding_window)
add_leetcode_node(monotonic_stack)
add_leetcode_node(bfs_grid)

# Install library
install(TARGETS leetcode_solutions DESTINATION lib/${PROJECT_NAME})

# ---- Tests ----
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()

  add_executable(test_leetcode test/test_leetcode.cpp)
  target_link_libraries(test_leetcode leetcode_solutions)
  target_include_directories(test_leetcode PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
  install(TARGETS test_leetcode DESTINATION lib/${PROJECT_NAME})
endif()

ament_package()
