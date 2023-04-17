from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(package="turtlesim", executable="turtlesim_node", remappings=[("/turtle1/cmd_vel", "cmd_vel")]),
        Node(package="teleop_twist_keyboard", executable="teleop_twist_keyboard" )
    ])