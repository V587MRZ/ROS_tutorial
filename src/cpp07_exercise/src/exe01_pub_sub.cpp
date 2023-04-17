/*
   需求：订阅窗口1中的乌龟速度，然后生成控制窗口2乌龟运动的指令并发布。
   步骤：
       1.包含头文件；
       2.初始化 ROS2 客户端；
       3.定义节点类；
          3-1.创建控制第二个窗体乌龟运动的发布方；
          3-2.创建订阅第一个窗体乌龟pose的订阅方；
          3-3.根据订阅的乌龟的速度生成控制窗口2乌龟运动的速度消息并发布。
       4.调用spin函数，并传入节点对象指针；
       5.释放资源。
*/
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
class ExePubSub : public rclcpp::Node
{
public:
    ExePubSub() : Node("demo01_pub_sub")
    {
        // // 创建订阅第一个窗体乌龟pose的订阅方；
        subscription_ = this->create_subscription<turtlesim::msg::Pose>(
            "/t1/turtle1/pose", 1, std::bind(&ExePubSub::poseCallback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
            "/t2/turtle1/cmd_vel", 1);
    }

private:
    void poseCallback(const turtlesim::msg::Pose &pose)
    {
        geometry_msgs::msg::Twist twist;
        twist.angular.z = -(pose.angular_velocity);
        twist.linear.x = pose.linear_velocity;
        publisher_->publish(twist);

    }
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ExePubSub>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}