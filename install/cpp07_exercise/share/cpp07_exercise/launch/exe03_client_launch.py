from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类---------------------
from launch.actions import ExecuteProcess
from launch.substitutions import FindExecutable
# 参数声明与获取
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# 文件包含相关---------------------------------
#from launch.actions import IncludeLaunchDescription
#from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关-------------------------------------
#from launch_ros.actions import PushRosNamespace
#from launch.actions import GroupAction
# 事件相关-------------------------------------
#from launch.event_handlers import OnProcessStart, OnProcessExit
#from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo
# 获取功能包下share目录路径---------------------
#from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    x = 8.54
    y = 9.54
    theta = 0.0
    name = "t2"
    spawn = ExecuteProcess(
        cmd = [FindExecutable(name='ros2'), 'service', 'call', '/spawn', 'turtlesim/srv/Spawn', 
               '\"{x: ' + str(x) + ', y: ' + str(y) + ', theta: ' + str(theta) + ', name: ' + name + '}\"'],
        output='screen',
        shell=True)
    client = Node(package='cpp07_exercise', executable='exe03_client', 
                  arguments=[str(x), str(y), str(theta)])
    return LaunchDescription([spawn, client])