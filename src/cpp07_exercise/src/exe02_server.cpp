#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
using base_interfaces_demo::srv::Distance;
class Exe03Client : public rclcpp::Node
{
public:
    Exe03Client() : Node("exe03_server_node")
    {
        pose_sub = this->create_subscription<turtlesim::msg::Pose>(
            "turtle1/pose", 10, std::bind(&Exe03Client::PoseCallBack, this,std::placeholders::_1));
        distance_server = this->create_service<Distance>(
            "distance", std::bind(&Exe03Client::DistanceCallback, this,std::placeholders::_1,std::placeholders::_2));
    }
    void PoseCallBack(const turtlesim::msg::Pose::SharedPtr pose)
    {
        turtle1_x = pose->x;
        turtle1_y = pose->y;
    }
    void DistanceCallback(const Distance::Request::SharedPtr request,
                          const Distance::Response::SharedPtr response)
    {
        float goal_x = request->x;
        float goal_y = request->y;

        // 距离计算
        float x = goal_x - turtle1_x;
        float y = goal_y - turtle1_y;
        // 将结果设置到响应
        response->distance = std::sqrt(x * x + y * y);
        RCLCPP_INFO(this->get_logger(),"目标坐标:(%.2f,%.2f),距离:%.2f",goal_x,goal_y,response->distance);

    }

private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_sub;
    rclcpp::Service<base_interfaces_demo::srv::Distance>::SharedPtr distance_server;
    float turtle1_x;
    float turtle1_y;
};
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Exe03Client>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}