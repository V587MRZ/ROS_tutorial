from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(package="turtlesim", executable="turtlesim_node", name="sim"),
        Node(package="turtlesim", executable="turtlesim_node", name="sim1"),
        Node(package="turtlesim", executable="turtlesim_node", namespace="ns1"),
        Node(package="turtlesim", executable="turtlesim_node")
    ])
