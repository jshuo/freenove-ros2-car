"""Launch all Freenove car hardware nodes.

Usage:
  ros2 launch freenove_car_bringup car.launch.py
  ros2 launch freenove_car_bringup car.launch.py simulate:=true
"""
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def launch_setup(context, *args, **kwargs):
    simulate = LaunchConfiguration('simulate').perform(context).lower() == 'true'
    params_file = LaunchConfiguration('params_file').perform(context)

    if not params_file:
        config_dir = get_package_share_directory('freenove_car_bringup')
        yaml_file = 'car_params_sim.yaml' if simulate else 'car_params.yaml'
        params_file = PathJoinSubstitution([config_dir, 'config', yaml_file])

    return [
        Node(
            package='freenove_car_core',
            executable='base_node',
            name='freenove_base',
            output='screen',
            parameters=[params_file],
        ),
        Node(
            package='freenove_car_core',
            executable='sensor_node',
            name='freenove_sensors',
            output='screen',
            parameters=[params_file],
        ),
        Node(
            package='freenove_car_core',
            executable='actuator_node',
            name='freenove_actuators',
            output='screen',
            parameters=[params_file],
        ),
        Node(
            package='freenove_car_core',
            executable='camera_node',
            name='freenove_camera',
            output='screen',
            parameters=[params_file],
        ),
    ]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'simulate',
            default_value='false',
            description='Run in simulation mode (no hardware required)'),
        DeclareLaunchArgument(
            'params_file',
            default_value='',
            description='Path to custom YAML parameter file'),
        OpaqueFunction(function=launch_setup),
    ])
