#include "rclcpp/rclcpp.hpp"
#include "base_interfaces_demo/srv/distance.hpp"
using base_interfaces_demo::srv::Distance;
using namespace std::chrono_literals;
class Exe03Client : public rclcpp::Node
{
public:
    Exe03Client() : Node("exe03_client_node")
    {
        client_ = this->create_client<Distance>("distance");
    }
    bool connect_server()
    {
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_INFO(this->get_logger(), "客户端退出！");
                return false;
            }

            RCLCPP_INFO(this->get_logger(), "服务连接中，请稍候...");
        }
        return true;
    }
    rclcpp::Client<base_interfaces_demo::srv::Distance>::FutureAndRequestId send_distance(
        float x, float y, float theta)
    {
        auto distance_request = std::make_shared<Distance::Request>();
        distance_request->x = x;
        distance_request->y = y;
        distance_request->theta = theta;
        return client_->async_send_request(distance_request);
    }

private:
    rclcpp::Client<Distance>::SharedPtr client_;
};
int main(int argc, char **argv)
{
    if (argc != 5)
    {
        RCLCPP_ERROR(rclcpp::get_logger("exe03_client_node"), "Invalid number of arguments");
        return 1;
    }
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Exe03Client>();
    float x = atof(argv[1]);
    float y = atof(argv[2]);
    float theta = atof(argv[3]);
    bool flag = node->connect_server();
    if (!flag)
    {
        RCLCPP_INFO(node->get_logger(), "服务连接失败!");
        return 1;
    }
    auto future = node->send_distance(x, y, theta);
    if (rclcpp::spin_until_future_complete(node, future) == rclcpp::FutureReturnCode::SUCCESS)
        RCLCPP_INFO(node->get_logger(), "两只乌龟相距%.2f米。", future.get()->distance);
    else
        RCLCPP_INFO(node->get_logger(), "获取距离服务失败!");
    rclcpp::shutdown();
    return 0;
}