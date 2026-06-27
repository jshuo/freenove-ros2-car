cmake_minimum_required(VERSION 3.8)
project(freenove_car_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)

# Motor speeds: 4 individual wheel duties
set(msg_files
  msg/MotorCommand.msg
  msg/ServoCommand.msg
  msg/InfraredState.msg
  msg/LEDCommand.msg
  msg/SensorState.msg
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES builtin_interfaces
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()
